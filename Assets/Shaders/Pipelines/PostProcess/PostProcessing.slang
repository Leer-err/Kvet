import Mesh;
import Dithering;
import Effects;

struct VertexOutput {
    float4 viewport_position : SV_Position;
    float2 uv : TEXCOORD;
};

struct PostProcessingData {
    int2 camera_dimensions;
    uint32_t texture_index;
    uint32_t sampler_index;
    
    //dithering
    float spread;
    // uint32_t dithering_order;

    //color quantization
    uint32_t color_count;

    // bool enable_dithering;
    // bool enable_color_quantization;
};

struct PushConstants{
    Mesh<Vertex> mesh;

    PostProcessingData* parameters;
};

[vk::push_constant]
PushConstants constants;

struct VertexTransform: IVertexTransform {
    typealias VertexIn = Vertex; 
    typealias VertexOut = VertexOutput; 

    VertexOut transform(VertexIn vertex){
        VertexOut out;

        out.viewport_position = float4(vertex.position.xy, 1, 1);
        out.uv = vertex.uv;

        return out;
    }
};

[shader("mesh")]
[outputtopology("triangle")]
[numthreads(GROUP_SIZE, 1, 1)]
void mesh_main( 
    in uint3 group_tid : SV_GroupThreadID,
    in uint3 group_index : SV_GroupID,
    out vertices VertexOutput vertices[64],
    out indices uint3 triangles[96])
{
    let transformer = VertexTransform();

    mesh_transform(constants.mesh, group_index.x, group_tid.x, transformer, vertices, triangles);
}

[[vk::binding(0, 0)]]
Texture2D textures[];
[[vk::binding(1, 0)]]
SamplerState samplers[];

[shader("pixel")]
float4 pixel_main(in VertexOutput data)
{
    int2 pixel_coordinates = data.uv * constants.parameters->camera_dimensions;

    float3 color = textures[constants.parameters->texture_index].Sample(samplers[0], data.uv).rgb;

    color = dithering8x8(color, constants.parameters->spread, pixel_coordinates);

    return float4(quantize_color(color.xyz, constants.parameters->color_count), 1);
}