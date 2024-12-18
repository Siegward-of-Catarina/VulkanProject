Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstBuff : register(b0)
{
  matrix mtxProj;
  matrix mtxView;
  matrix mtxWorld;
  float4 Diffuse;
};

struct PS_INPUT
{
  float4 Pos : SV_POSITION;
  float2 Tex : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_Target
{
  return txDiffuse.Sample(samLinear, input.Tex)*Diffuse;
}