struct PSinput
{
    float4 pos : SV_Position;
    [[vk::location(0)]]float4 color : COLOR;
};

float4 main(PSinput input) : SV_TARGET
{
	return input.color;
}