cbuffer cbChangesPerObject : register( b0 ) {
    matrix mvp;
    matrix world;    
};

struct VS_Input {
    float4 position  : POSITION;
    float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4x4 world : WORLD;
	float4 color : COLOR;
};

struct PS_Input {
    float4 pos  : SV_POSITION;
	float3 normal : NORMAL;
    float4 color : COLOR;
};


PS_Input VS_Main( VS_Input vertex ) {
    PS_Input vsOut = ( PS_Input )0;
	vertex.position.w = 1.0;
    vsOut.pos = mul( vertex.position, vertex.world);
	vsOut.pos = mul(vsOut.pos, mvp);
    vsOut.color = vertex.color;
	vsOut.normal = normalize(mul(vertex.normal, (float3x3)world));
    return vsOut;
}
