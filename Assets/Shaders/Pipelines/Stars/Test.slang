import Camera;
import Mesh;

struct VertexOutput{
    float4 viewport_position : SV_POSITION;
    float3 position;
};

struct VertexInput{
    float3 position;
    float2 uv;
};

struct PushConstants{
    Mesh<VertexInput> mesh;

    CameraData* camera_data;
    float4x4 model;
};

[vk::push_constant]
PushConstants constants;

static const uint GROUP_SIZE = 32;

[shader("mesh")]
[outputtopology("triangle")]
[numthreads(32, 1, 1)]
void mesh_main(
    in uint3 group_tid : SV_GroupThreadID,
    in uint3 group_index : SV_GroupID,
    out vertices VertexOutput vertices[64],
    out indices uint3 triangles[96])
{
    let meshlet_index = group_index.x;
    Meshlet meshlet = constants.mesh.meshlets[meshlet_index];
    SetMeshOutputCounts(meshlet.vertex_count, meshlet.triangle_count);

    float4x4 mvp =  mul(constants.camera_data->view_projection, constants.model);

    for(int i = group_tid.x; i < meshlet.vertex_count; i += GROUP_SIZE){
        VertexInput vertex = constants.mesh.getVertex(meshlet_index, i);

        float4 vertex_position = float4(vertex.position, 1);

        vertices[i].viewport_position = mul(mvp, vertex_position);
        vertices[i].position = vertex_position.xyz;
    }

    for(int i = group_tid.x; i < meshlet.triangle_count; i += GROUP_SIZE){
        triangles[i] = constants.mesh.getTriangle(meshlet_index, i);
    }
}

[shader("pixel")]
float4 pixel_main(in VertexOutput data)
{
    return float4(1,0,0,1);
}