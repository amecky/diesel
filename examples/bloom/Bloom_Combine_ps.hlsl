cbuffer cbChangesPerFrame : register(b1) {
	float BloomSaturation;
	float BloomIntensity;
	float OriginalSaturation;
	float OriginalIntensity;
};

struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D ColorMapSampler : register(t0);
Texture2D BlurSampler : register(t1);
SamplerState colorSampler : register(s0);

float4 AdjustSaturation(float4 color, float saturation) {
	float grey = dot(color, float4(0.3, 0.59, 0.11, 1.0));
	return lerp(grey, color, saturation);
}

float4 PS_Main(PS_Input input) : SV_TARGET{
	float4 bloomColor = BlurSampler.Sample(colorSampler, input.tex);
	float4 originalColor = ColorMapSampler.Sample(colorSampler, input.tex);
	bloomColor = AdjustSaturation(bloomColor, BloomSaturation) * BloomIntensity;
	originalColor = AdjustSaturation(originalColor, OriginalSaturation) * OriginalIntensity;
	originalColor *= (1 - saturate(bloomColor));
	return originalColor + bloomColor;
}
