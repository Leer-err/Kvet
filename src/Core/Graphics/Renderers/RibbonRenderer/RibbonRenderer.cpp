#include "RibbonRenderer.h"

#include <vulkan/vulkan_core.h>

#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstring>
#include <numeric>
#include <ranges>
#include <vector>

#include "BufferBuilder.h"
#include "CommandBuffer.h"
#include "EngineData.h"
#include "FrameGraph.h"
#include "Particle.h"

namespace views = std::views;

namespace Graphics {

struct PipellineRange {
    size_t start;
    size_t count;
};

struct RibbonMeshlet {
    uint32_t first_index;
    uint32_t index_count;
    uint32_t ribbon_index;
};

struct RibbonData {
    VkDeviceAddress ribbon_nodes;
    VkDeviceAddress ribbon_meshlets;
    VkDeviceAddress meshlet_indices;
    VkDeviceAddress camera_data;
};

RibbonRenderer::RibbonRenderer(Device& device, const EngineData& engine_data,
                               BufferHandle camera_buffer)
    : engine_data(engine_data),
      camera_buffer(camera_buffer),
      nodes(createNodeBuffer(device)),
      meshlets(createMeshletsBuffer(device)),
      meshlet_indices(createMeshletIndicesBuffer(device)) {}

void RibbonRenderer::render(FrameGraph& frame_graph, RenderWorld& world) {
    auto ribbons = std::vector<Ribbon>({Ribbon{{}, {0, 1, 2}}});
    auto ribbon_nodes =
        std::vector<RibbonNode>({{{-1, 0, 1}}, {{0, 0, 1}}, {{1, 0, 1}}});

    nodes->update(std::bit_cast<uint8_t*>(ribbon_nodes.data()),
                  sizeof(RibbonNode) * ribbon_nodes.size(), 0);

    auto ribbon_comparator = [ribbons](size_t a, size_t b) {
        const auto& first_ribbon = ribbons[a];
        const auto& second_ribbon = ribbons[b];

        return first_ribbon.pipeline->getPipeline() <
               second_ribbon.pipeline->getPipeline();
    };
    auto render_order = std::vector<size_t>(ribbons.size());
    std::iota(render_order.begin(), render_order.end(), 0u);
    std::ranges::sort(render_order, ribbon_comparator);

    auto pipeline_ranges = std::vector<PipellineRange>();
    auto& last_pipeline = ribbons[render_order[0]].pipeline;

    auto start = 0;
    auto size = 0;
    for (int i = 0; i < render_order.size(); i++) {
        auto& pipeline = ribbons[render_order[i]].pipeline;
        if (last_pipeline == pipeline) {
            size++;
        } else {
            last_pipeline = ribbons[render_order[i]].pipeline;
            pipeline_ranges.emplace_back(start, size);
            start = i;
            size = 0;
        }
    }
    pipeline_ranges.emplace_back(start, size);

    auto meshlet_count = generateMeshlets(ribbons);

    auto color_attachment = *engine_data.resource_manager.getTexture("Color");
    auto depth_attachment = *engine_data.resource_manager.getTexture("Depth");

    for (const auto& range : pipeline_ranges) {
        auto material_pipeline_count = range.count;
        auto material_ribbons = render_order | views::drop(range.start) |
                                views::take(material_pipeline_count);

        auto first_material_ribbon = *material_ribbons.begin();
        auto pipeline = ribbons[first_material_ribbon].pipeline;

        auto push_constants =
            RibbonData{nodes->getDeviceAddress(), meshlets->getDeviceAddress(),
                       meshlet_indices->getDeviceAddress(),
                       camera_buffer->getDeviceAddress()};

        pipeline =
            *engine_data.resource_manager.getPipeline("Shaders/RibbonPipeline");
        auto pass =
            GeneralPass("Ribbon", [meshlet_count, pipeline, push_constants,
                                   color_attachment,
                                   depth_attachment](const CommandBuffer& cmd) {
                RenderEnviroment env = {};

                env.render_target = color_attachment->getView();
                env.depth_stencil = depth_attachment->getView();
                env.width = color_attachment->getWidth();
                env.height = color_attachment->getHeight();

                cmd.setPipeline(pipeline);
                cmd.bindRenderEnviroment(env);
                cmd.pushConstants(pipeline, &push_constants, 0);
                cmd.draw(meshlet_count, 1);
                cmd.unbindRenderEnviroment();
            });

        frame_graph.addGeneralPass(pass);
    }
}

size_t RibbonRenderer::generateMeshlets(const std::vector<Ribbon>& ribbons) {
    constexpr size_t MAX_MESHLET_LINE_SIZE = 32;
    constexpr size_t MAX_MESHLET_SIZE = MAX_MESHLET_LINE_SIZE + 2;
    constexpr size_t MESHLET_UNIQUE_NODES_COUNT = MAX_MESHLET_LINE_SIZE - 1;

    auto node_indices_ptr =
        std::bit_cast<uint32_t*>(meshlet_indices->getHostAddress());
    auto meshlets_ptr =
        std::bit_cast<RibbonMeshlet*>(meshlets->getHostAddress());

    auto current_meshlet = 0u;
    auto current_index = 0u;

    for (auto ribbon_index = 0u; ribbon_index < ribbons.size();
         ribbon_index++) {
        const auto& ribbon = ribbons[ribbon_index];

        auto node_count = ribbon.nodes.size();
        auto ribbon_meshlets_count =
            (node_count + MAX_MESHLET_LINE_SIZE - 1) / MAX_MESHLET_LINE_SIZE;

        for (int i = 0; i < ribbon_meshlets_count; i++) {
            auto is_first_meshlet = i == 0;
            auto is_last_meshlet = i == (ribbon_meshlets_count - 1);
            auto meshlet_first_index_offset = current_index;

            auto first_node_index = i * MESHLET_UNIQUE_NODES_COUNT - 1;
            if (is_first_meshlet) {
                first_node_index = 0;

                memcpy(&node_indices_ptr[current_index], &ribbon.nodes[0],
                       sizeof(uint32_t));
                current_index++;
            }
            auto first_node_ptr = &ribbon.nodes[first_node_index];

            auto meshlet_line_length = std::min(
                MAX_MESHLET_LINE_SIZE, ribbon.nodes.size() - first_node_index);

            auto copy_count = meshlet_line_length + 2;
            if (is_last_meshlet) copy_count--;
            if (is_first_meshlet) copy_count--;
            memcpy(&node_indices_ptr[current_index], first_node_ptr,
                   copy_count * sizeof(uint32_t));

            current_index += copy_count;

            if (is_last_meshlet) {
                auto node_ptr =
                    &ribbon.nodes[first_node_index + copy_count - 1];

                memcpy(&node_indices_ptr[current_index], node_ptr,
                       sizeof(uint32_t));
                current_index++;
            }

            meshlets_ptr[current_meshlet] = RibbonMeshlet{
                meshlet_first_index_offset,
                static_cast<uint32_t>(meshlet_line_length), ribbon_index};

            current_meshlet++;
        }
    }

    return current_meshlet;
}

BufferHandle RibbonRenderer::createNodeBuffer(Device& device) {
    return BufferBuilder(sizeof(RibbonNode) * 3)
        .isConstantBuffer()
        .isDeviceAddressable()
        .isChained()
        .isCPUWritable(true)
        .create(device)
        .getResult();
}

BufferHandle RibbonRenderer::createMeshletIndicesBuffer(Device& device) {
    return BufferBuilder(sizeof(uint32_t) * 5)
        .isConstantBuffer()
        .isDeviceAddressable()
        .isChained()
        .isCPUWritable(true)
        .create(device)
        .getResult();
}

BufferHandle RibbonRenderer::createMeshletsBuffer(Device& device) {
    return BufferBuilder(sizeof(RibbonMeshlet))
        .isConstantBuffer()
        .isDeviceAddressable()
        .isChained()
        .isCPUWritable(true)
        .create(device)
        .getResult();
}

};  // namespace Graphics