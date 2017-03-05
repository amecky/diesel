cbuffer cbChangesPerObject : register( b0 ) {
    matrix mvp;
    matrix world;    
};

struct VS_Input {
    float4 position  : POSITION;
    float2 texcoord : TEXCOORD;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float3 instancepos : NORMAL1;
	float4 icolor : COLOR1;
};

struct PS_Input {
    float4 pos  : SV_POSITION;
	float3 normal : NORMAL;
    float4 color : COLOR;
};


PS_Input VS_Main( VS_Input vertex ) {
    PS_Input vsOut = ( PS_Input )0;
	vertex.position.w = 1.0;
	float4 p = vertex.position;
	p.x += vertex.instancepos.x;
	p.y += vertex.instancepos.y;
	p.z += vertex.instancepos.z;
    vsOut.pos = mul( p, world);
	vsOut.pos = mul(vsOut.pos, mvp);
    vsOut.color = vertex.color * vertex.icolor;
	vsOut.normal = normalize(mul(vertex.normal, (float3x3)world));
    return vsOut;
}
