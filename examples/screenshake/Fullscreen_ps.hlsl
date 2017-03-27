cbuffer cbChangesPerFrame : register(b0) {
	float4 data;
};

struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);

float4 PS_Main( PS_Input frag ) : SV_TARGET {
	float4 c = colorMap_.Sample(colorSampler_, frag.tex);
	return c;
}

