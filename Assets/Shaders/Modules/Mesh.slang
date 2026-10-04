module Mesh;

public typedef uint MeshletIndex;

typedef uint32_t VertexIndex;
typedef uint8_t TriangleIndex;

public struct Meshlet{
    uint32_t vertex_offset; 
    uint32_t triangle_offset;

    public uint8_t vertex_count;
    public uint8_t triangle_count;
};

public struct Vertex{
    public float3 position;
    public float2 uv;
    public float3 normal;
};

public struct PixelVertex{
    public float4 viewport_position : SV_POSITION;
    public float3 position;
    public float3 world_position;
    public float3 normal;
};

public struct Mesh<T>{
    T* vertices;
    TriangleIndex* meshlet_triangles;
    VertexIndex* meshlet_vertices;
    public Meshlet* meshlets;

    public T getVertex(in MeshletIndex meshlet_index, in VertexIndex vertex_index){
        Meshlet meshlet = meshlets[meshlet_index];
        VertexIndex meshlet_vertex_index = meshlet.vertex_offset + vertex_index;
        VertexIndex mesh_vertex_index = meshlet_vertices[meshlet_vertex_index];
        return vertices[mesh_vertex_index];
    }

    public uint3 getTriangle(in MeshletIndex meshlet_index, in TriangleIndex triangle_index){
        Meshlet meshlet = meshlets[meshlet_index];
        uint meshlet_triangle_index = meshlet.triangle_offset + triangle_index * 3;

        uint3 triangle;
        triangle.x = meshlet_triangles[meshlet_triangle_index];
        triangle.y = meshlet_triangles[meshlet_triangle_index + 1];
        triangle.z = meshlet_triangles[meshlet_triangle_index + 2];

        return triangle;
    }
};

public interface IVertexTransform {
    public associatedtype VertexIn;
    public associatedtype VertexOut;

    public VertexOut transform(VertexIn vertex);
};

public static const uint GROUP_SIZE = 32;

public void mesh_transform<Transformer: IVertexTransform>(
    in Mesh<Transformer.VertexIn> mesh,
    in uint meshlet_index,
    in uint invocation_index,
    in Transformer transformer, 
    out vertices Transformer.VertexOut vertices[64],
    out indices uint3 triangles[96])
{
    Meshlet meshlet = mesh.meshlets[meshlet_index];
    SetMeshOutputCounts(meshlet.vertex_count, meshlet.triangle_count);

    for(uint i = invocation_index; i < meshlet.vertex_count; i += GROUP_SIZE){
        let vertex = mesh.getVertex(meshlet_index, i);

        vertices[i] = transformer.transform(vertex);
    }

    for(uint i = invocation_index; i < meshlet.triangle_count; i += GROUP_SIZE){
        triangles[i] = mesh.getTriangle(meshlet_index, i);
    }
}