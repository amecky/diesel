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

