struct Vertex
{
    float2 position;
    float3 color;
};

static Vertex verties[6] =
{
    {
        { .5f, -.5f },
        { 1.0f, .0f, .0f }
    },
    {
        { .5f, .5f },
        { .0f, 1.0f, .0f }
    },
    {
        { -.5f, .5f },
        { .0f, .0f, 1.0f }
    },
    {
        { .5f, -.5f },
        { 1.0f, .0f, .0f }
    },
    {
        { -.5f, .5f },
        { .0f, .0f, 1.0f }
    },
    {
        { -.5f, -.5f },
        { .0f, 1.0f, 1.0f }
    }
};

struct VSoutput
{
    float4 position : SV_Position;
    [[vk::location(0)]]float4 color : COLOR0;
};

VSoutput main(uint vertex_index : SV_VertexID)
{
    VSoutput v_out = (VSoutput) 0;
    v_out.color = float4(verties[vertex_index].color, 1.0f);
    v_out.position = float4(verties[vertex_index].position, 0.0, 1.0);
    return v_out;
}