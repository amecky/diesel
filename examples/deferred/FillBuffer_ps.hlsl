struct PS_Input {
    float4 pos  : SV_POSITION;
    float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
SamplerState baseSampler : register(s0);

float4 PS_Main( PS_Input frag ) : SV_TARGET {

	float3 base = colorMap.Sample(baseSampler, frag.texcoord).rgb;
	float3 bump = normalMap.Sample(baseSampler, frag.texcoord).xyz * 2.0 - 1.0;

	float3 normal = frag.tangent * bump.x + frag.binormal * bump.y + frag.normal * bump.z;
	normal = normalize(normal);
	// Fill the buffers
	//Out.base = base;
	//Out.normal = normal * 0.5 + 0.5;
    //return colorMap_.Sample(colorSampler_, frag.texcoord);
	return float4(normal, 1.0);
}

