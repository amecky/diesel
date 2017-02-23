struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float4 color : COLOR0;
};


Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);

float4 PS_Main(PS_Input frag) : SV_TARGET{
	return colorMap_.Sample(colorSampler_, frag.tex0) * frag.color;
}
