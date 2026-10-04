module Effects;

public float fresnel(float3 camera_position, float3 vertex_position, float3 normal, float size, float strength, float bias){
    float3 vertex_direction = normalize(vertex_position - camera_position);
    return bias + strength * pow(1 + dot(vertex_direction, normal), 1 / size);
}