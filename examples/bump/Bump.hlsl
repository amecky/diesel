cbuffer cbChangesPerObject : register( b0 ) {
    matrix mvp;
    matrix world;    
	float3 eyePos;
	float padding;
};

struct VS_Input {
    float4 position  : POSITION;
    float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct PS_Input {
    float4 pos  : SV_POSITION;
    float2 texcoord : TEXCOORD0;
	float3 Light : TEXCOORD1;
	float3 View : TEXCOORD2;
};


PS_Input VS_Main( VS_Input vertex ) {
	float3 vecLightDir = float3(0, 0, 1);
    PS_Input vsOut = ( PS_Input )0;
	//vertex.position.w = 1.0;
	vsOut.pos = mul( vertex.position, world);
	vsOut.pos = mul(vsOut.pos, mvp);
	float3x3 worldToTangentSpace;
    vsOut.texcoord = vertex.texcoord;
	worldToTangentSpace[0] = normalize(mul(vertex.normal, (float3x3)world));
	worldToTangentSpace[1] = normalize(mul(vertex.tangent, (float3x3)world));
	worldToTangentSpace[2] = normalize(mul(cross(vertex.normal,vertex.tangent), (float3x3)world));
	float4 PosWorld = mul(vertex.position, world);
	// Pass out light and view directions, pre-normalized
	vsOut.Light = normalize(mul(worldToTangentSpace, vecLightDir));
	vsOut.View = normalize(mul(worldToTangentSpace, eyePos - PosWorld));
    return vsOut;
}

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
SamplerState colorSampler_ : register(s0);

float4 PS_Main( PS_Input frag ) : SV_TARGET {
	
	float4 Color = colorMap.Sample(colorSampler_, frag.texcoord);
	// Get the Color of the normal. The color describes the direction of the normal vector
	// and make it range from 0 to 1.
	float3 N = (2.0 * (normalMap.Sample(colorSampler_, frag.texcoord))) - 1.0;
	// diffuse
	float D = saturate(dot(N, frag.Light));
	// reflection
	float3 R = normalize(2 * D * N - frag.Light);
	// specular
	float S = pow(saturate(dot(R, frag.View)), 2);
	// calculate light (ambient + diffuse + specular)
	const float4 Ambient = float4(0.1, 0.1, 0.1, 1.0);
	return Color*Ambient + Color * D + Color*S;
}

