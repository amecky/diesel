cbuffer cbChangesPerFrame : register(b0) {
	float BloomThreshold;
	float padding1;
	float padding2;
	float padding3;
};

struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D ColorMapSampler : register(t0);
SamplerState colorSampler : register(s0);

float4 PS_Main(PS_Input input) : SV_TARGET{
	float4 c = ColorMapSampler.Sample(colorSampler, input.tex);
	return saturate((c - BloomThreshold) / (1 - BloomThreshold));
}
