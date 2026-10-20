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
    let vertex_count = (meshlet.index_count) * 2;
    let triangle_count = (meshlet.index_count) * 2 - 2;
    SetMeshOutputCounts(vertex_count, triangle_count);

    if(group_tid.x > meshlet.index_count - 1) return;
    let meshlet_node_index = meshlet.first_index + group_tid.x + 1;
    let current_node_index = constants.meshlet_indices[meshlet_node_index];
    let current_node = constants.nodes[current_node_index];
    let previous_node_index = constants.meshlet_indices[meshlet_node_index - 1];
    let previous_node = constants.nodes[previous_node_index];
    let next_node_index = constants.meshlet_indices[meshlet_node_index + 1];
    let next_node = constants.nodes[next_node_index];

    let camera_vector = normalize(current_node.position - constants.camera.position);
    let prev_tangent = normalize(current_node.position - previous_node.position);
    let prev_normal = normalize(cross(prev_tangent, camera_vector));
    let next_tangent = normalize(next_node.position - current_node.position);
    let next_normal = normalize(cross(next_tangent, camera_vector));

    float3 normal = float3(0,1,0);
    if (current_node_index == previous_node_index) normal = next_normal;
    else if (current_node_index == next_node_index) normal = prev_normal;
    else normal = normalize(prev_normal + next_normal);

    let vertex_offset = normal * 0.1;
    vertices[group_tid.x * 2].viewport_position = mul(constants.camera.view_projection, float4(current_node.position + vertex_offset, 1));
    vertices[group_tid.x * 2 + 1].viewport_position = mul(constants.camera.view_projection, float4(current_node.position - vertex_offset, 1));

    if(group_tid.x == meshlet.index_count - 1) return;
    triangles[group_tid.x * 2] = uint3(group_tid.x * 2, group_tid.x * 2 + 2, group_tid.x * 2 + 1);
    triangles[group_tid.x * 2 + 1] = uint3(group_tid.x * 2 + 1, group_tid.x * 2 + 2, group_tid.x * 2 + 3);
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