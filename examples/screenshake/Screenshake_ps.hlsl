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
	float3 greyscale = dot(c.rgb, float3(0.30, 0.59, 0.11));
	float3 rgb = lerp(c.rgb, greyscale, data.x);
	return float4(rgb, c.a);
}

