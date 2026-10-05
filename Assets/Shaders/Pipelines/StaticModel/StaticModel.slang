import Camera;
import Mesh;

struct StaticModelData
{
    float4x4 model;

    uint32_t albedo_descriptor;
    uint32_t sampler_descriptor;
};

struct VertexOutput{
    float4 viewport_position : SV_POSITION;
    float2 uv : TEXCOORD;
};

struct PushConstants{
    Mesh<Vertex> mesh;

    CameraData* camera_data;
    StaticModelData* model_data;
};

[vk::push_constant]
PushConstants constants;

struct VertexTransform: IVertexTransform {
    typealias VertexIn = Vertex; 
    typealias VertexOut = VertexOutput; 

    float4x4 mvp;

    __init() { mvp = mul(constants.camera_data->view_projection, constants.model_data->model); }

    VertexOut transform(VertexIn vertex){
        VertexOut out;

        float4 vertex_position = float4(vertex.position, 1);

        out.viewport_position = mul(mvp, vertex_position);
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
float4 pixel_main(in float2 uv : TEXCOORD) : SV_Target
{
    uint32_t texture_index = constants.model_data.albedo_descriptor;
    uint32_t sampler_index = constants.model_data.sampler_descriptor;
    return textures[texture_index].Sample(samplers[sampler_index], uv);
}