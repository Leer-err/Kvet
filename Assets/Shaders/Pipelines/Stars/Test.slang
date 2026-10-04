import Camera;
import Mesh;
import Effects;

struct Data{
    float time;
};

struct PushConstants{
    Mesh<Vertex> mesh;

    CameraData* camera_data;
    Data* data;
    float4x4 model;
};

struct VertexTransform: IVertexTransform {
    typealias VertexIn = Vertex; 
    typealias VertexOut = PixelVertex; 

    float4x4 mvp;

    __init() { mvp = mul(constants.camera_data->view_projection, constants.model); }

    VertexOut transform(VertexIn vertex){

        VertexOut out;

        float4 vertex_position = float4(vertex.position, 1);

        out.viewport_position = mul(mvp, vertex_position);
        out.world_position = mul(constants.model, vertex_position).xyz;
        out.normal = normalize(mul(float3x3(constants.model), vertex.normal));
        out.position = vertex_position.xyz;

        return out;
    }
};

[vk::push_constant]
PushConstants constants;

[shader("mesh")]
[outputtopology("triangle")]
[numthreads(GROUP_SIZE, 1, 1)]
void mesh_main(
    in uint3 group_tid : SV_GroupThreadID,
    in uint3 group_index : SV_GroupID,
    out vertices PixelVertex vertices[64],
    out indices uint3 triangles[96])
{
    let transformer = VertexTransform();

    mesh_transform<VertexTransform>(constants.mesh, group_index.x, group_tid.x, transformer, vertices, triangles);
}

static const float bias = 0;
static const float strength = 0.4;
static const float size = 0.4;

[shader("pixel")]
float4 pixel_main(in PixelVertex data)
{
    float fresnel_factor = fresnel(constants.camera_data.position, data.world_position, data.normal, 0.5 * sin(constants.data.time) + 1, strength, bias);

    return lerp(float4(0,0,0,1), float4(0.063, 0.007, 0.305, 1), fresnel_factor);
}