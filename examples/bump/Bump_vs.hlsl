cbuffer cbChangesPerObject : register( b0 ) {
    matrix mvp;
    matrix world;    
};

struct VS_Input {
    float4 position  : POSITION;
    float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct PS_Input {
    float4 pos  : SV_POSITION;
    float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};


PS_Input VS_Main( VS_Input vertex ) {
    PS_Input vsOut = ( PS_Input )0;
	vertex.position.w = 1.0;
    vsOut.pos = mul( vertex.position, world);
	vsOut.pos = mul(vsOut.pos, mvp);
    vsOut.texcoord = vertex.texcoord;
	vsOut.normal = mul(vertex.normal, (float3x3)world);
	vsOut.normal = normalize(vsOut.normal);
	vsOut.tangent = mul(vertex.tangent, (float3x3)world);
	vsOut.tangent = normalize(vsOut.tangent);
	vsOut.binormal = cross(vsOut.normal, vsOut.tangent);
	vsOut.binormal = normalize(vsOut.binormal);
    return vsOut;
}

