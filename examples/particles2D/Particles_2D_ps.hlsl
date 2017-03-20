struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float4 color : COLOR0;
};


Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);

float4 PS_Main(PS_Input frag) : SV_TARGET {
	float border_size = 0.2;
	float disc_radius = 0.3;
	float dx = 2 * frag.tex0.x - 1;
	float dy = 2 * frag.tex0.y - 1;
	float hyp = sqrt(dx * dx + dy * dy);
	float t = smoothstep(disc_radius + border_size, disc_radius - border_size, hyp);
	//float nonBorder = abs(hyp - 1) * 0.5;
	//nonBorder = pow(nonBorder, 2.0f);
	float4 outColor = frag.color;
	//outColor.a = outColor.a * lerp(1.0, 0.0, t) * frag.tex0.x;
	outColor.a = outColor.a * lerp(0.0, 1.0, t) * frag.tex0.x;
	return pow(outColor, 1.0f / 2.2f); // gamma correction

	//float4 c = frag.color;
	//float2 uv = frag.tex0;
	//float l = 1.0 - length(uv);
	//c.a = l;
	//return c;
	//return colorMap_.Sample(colorSampler_, frag.tex0) * frag.color;
}
