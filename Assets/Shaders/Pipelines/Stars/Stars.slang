import Camera;
import Mesh;
import Noise;
import Math;

struct VertexOutput{
    float4 viewport_position : SV_POSITION;
    float3 position;
};

struct StarsData
{
    float time;
    float star_density;
    float blinking_speed;
    float blink_strength;
};

struct PushConstants{
    Mesh<Vertex> mesh;

    CameraData* camera_data;
    StarsData* stars_data;
    float4x4 model;
};

[vk::push_constant]
PushConstants constants;

struct VertexTransform: IVertexTransform {
    typealias VertexIn = Vertex; 
    typealias VertexOut = VertexOutput; 

    VertexOut transform(VertexIn vertex){
        float4x4 vp = constants.camera_data->view_projection_no_camera_position;

        VertexOut out;

        float4 vertex_position = float4(vertex.position, 1);

        out.viewport_position = mul(vp, vertex_position);
        out.position = vertex_position.xyz;

        return out;
    }
};

[shader("mesh")]
[outputtopology("triangle")]
[numthreads(32, 1, 1)]
void mesh_main(
    in uint3 group_tid : SV_GroupThreadID,
    in uint3 group_index : SV_GroupID,
    out vertices VertexOutput vertices[64],
    out indices uint3 triangles[96])
{
    let transformer = VertexTransform();

    mesh_transform(constants.mesh, group_index.x, group_tid.x, transformer, vertices, triangles);
}

[shader("pixel")]
float4 pixel_main(in VertexOutput data)
{
    StarsData stars_data = *constants.stars_data;

    float3 noise_position = normalize(data.position);
    float noise = 1 - voronoi3d(noise_position * stars_data.star_density).x;

    float stars = step(0.95, noise);

    float blink_offset = stars_data.time * stars_data.blinking_speed;
    float brightness_noise = fbm((noise_position * 10 + blink_offset) , 0.5);
    float star_brightness = remap(brightness_noise, 0, 1, 1 - stars_data.blink_strength, 1);

    stars = stars * star_brightness;

    return stars;
}