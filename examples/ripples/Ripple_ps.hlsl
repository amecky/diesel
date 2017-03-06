struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD;
};


Texture2D rtMap : register(t0);
Texture2D colorMap : register(t1);
SamplerState colorSampler : register(s0);

float4 PS_Main(PS_Input frag) : SV_TARGET{

	float2 t = frag.tex;

	//Look up the displacement amount.
	//It is in the range 0...1 for both red and green channels. Blue is strength of displacement.
	float4 displacement = rtMap.Sample(colorSampler, t);
	//Convert the displacement to the range -0.5...0.5 (taking into account overal strength)
	displacement.rg -= (0.5 * displacement.b);

	//Scale the strength - this is just some arbritrary value.
	float strength = displacement.b * 0.1;// 0.9994;

	//Offset the sceen texture coordinate by the amount of the displacement.
	frag.tex.x += (displacement.r * strength);
	frag.tex.y += (displacement.g * strength);

	// Look up into the main texture.
	return colorMap.Sample(colorSampler, frag.tex);


}
