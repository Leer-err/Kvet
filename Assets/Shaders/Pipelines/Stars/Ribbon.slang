import Camera;

struct RibbonMeshlet{
    uint32_t first_index;
    uint32_t index_count;
    uint32_t ribbon_index;
};

struct RibbonNode{
    float3 position;
};

struct VertexOutput{
    float4 viewport_position : SV_POSITION;
};

struct PushConstants{
    RibbonNode* nodes;
    RibbonMeshlet* meshlets;
    uint32_t* meshlet_indices;
    CameraData* camera;
};

[vk::push_constant]
PushConstants constants;

static const uint32_t GROUP_SIZE = 32;

[shader("mesh")]
[outputtopology("triangle")]
[numthreads(32, 1, 1)]
void mesh_main( 
    in uint3 group_tid : SV_GroupThreadID,
    in uint3 group_index : SV_GroupID,
    out vertices VertexOutput vertices[64],
    out indices uint3 triangles[96])
{
    let meshlet = constants.meshlets[group_index.x];
    let vertex_count = (meshlet.index_count - 2) * 2;
    let triangle_count = (meshlet.index_count - 2) * 2 - 2;
    SetMeshOutputCounts(vertex_count, triangle_count);

    if(group_tid.x > meshlet.index_count) return;
    let meshlet_node_index = meshlet.first_index + group_tid.x;
    let current_node_index = constants.meshlet_indices[meshlet_node_index];
    let current_node = constants.nodes[current_node_index];
    let previous_node_index = constants.meshlet_indices[meshlet_node_index];
    let previous_node = constants.nodes[current_node_index];

    let tangent = normalize(current_node.position - previous_node.position);
    let camera_vector = normalize(current_node.position - constants.camera.position);
    let normal = normalize(cross(tangent, camera_vector));

    let vertex_offset = normal * 0.1;
    vertices[group_tid.x * 2].viewport_position = mul(constants.camera.view_projection, float4(current_node.position + vertex_offset, 1));
    vertices[group_tid.x * 2 + 1].viewport_position = mul(constants.camera.view_projection, float4(current_node.position - vertex_offset, 1));

    // if(group_tid.x == meshlet.index_count) return;
    // triangles[group_tid.x] = uint3(group_tid.x * 2, group_tid.x * 2 + 1, group_tid.x * 2 + 2);
    // triangles[group_tid.x + 1] = uint3(group_tid.x * 2 + 1, group_tid.x * 2 + 2, group_tid.x * 2 + 3);
}

[[vk::binding(0, 0)]]
Texture2D textures[];
[[vk::binding(1, 0)]]
SamplerState samplers[];

[shader("pixel")]
float4 pixel_main(in float2 uv : TEXCOORD) : SV_Target
{
    return float4(1, 1, 1, 1);
}