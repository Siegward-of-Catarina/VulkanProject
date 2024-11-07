struct VSInput
{
    [[vk::location(0)]]float2 position : POSITIONT;
    [[vk::location(1)]]float3 color : COLOR0;
};

struct VSoutput
{
    float4 position : SV_Position;
    [[vk::location(0)]]float4 color : COLOR0;
};

VSoutput main(VSInput input)
{
    VSoutput v_out = (VSoutput) 0;
    v_out.color = float4(input.color, 1.0f);
    v_out.position = float4(input.position, 0.0, 1.0);
    return v_out;
}