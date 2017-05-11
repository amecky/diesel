cbuffer cbChangesPerFrame : register(b0) {
	matrix kernels;
	matrix weights;
	float2 direction;
	float2 data;
};

struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD;
};

Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);

float4 PS_Main( PS_Input frag ) : SV_TARGET {
	float4 color = float4(0.0,0.0,0.0,0.0);
	float2 coord = frag.tex;
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < 4; ++i) {
			int idx = i + j * 4;
			coord.x = frag.tex.x + direction.x * (kernels[j][i] * 1.0 / data.x);
			coord.y = frag.tex.y + direction.y * (kernels[j][i] * 1.0 / data.y);
			color += colorMap_.Sample(colorSampler_, coord.xy) * weights[j][i];
		}
	}
	return color;
}

