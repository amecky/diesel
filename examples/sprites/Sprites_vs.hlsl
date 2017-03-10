cbuffer cbChangesPerObject : register( b0 ) {
    matrix mvp;
    matrix world;    
};

struct VS_Input {
	float3 pos  : POSITION;
	float4 tex : COLOR0;
	float3 size : NORMAL0;
	float4 color : COLOR1;
};

struct GSPS_INPUT {
	float4 Pos : SV_POSITION;
	float4 Tex : COLOR0;
	float3 Size : NORMAL0;
	float4 Color : COLOR1;
};

GSPS_INPUT VS_Main(VS_Input vertex) {
	GSPS_INPUT vsOut = (GSPS_INPUT)0;
	vsOut.Pos = float4(vertex.pos, 1.0);
	vsOut.Tex = vertex.tex;
	vsOut.Color = vertex.color;
	vsOut.Size = vertex.size;
	return vsOut;
}

