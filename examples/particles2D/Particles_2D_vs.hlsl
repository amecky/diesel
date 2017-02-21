cbuffer cbChangesPerObject : register( b0 ) {
    matrix mvp;
    matrix world;    
};

struct VS_Input {
	float3 pos  : POSITION;
	float2 tex : TEXCOORD;
	float3 size : NORMAL;
	float4 color : COLOR;
};

struct GSPS_INPUT {
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Size : NORMAL;
	float4 Color : COLOR;
};

GSPS_INPUT VS_Main(VS_Input vertex) {
	GSPS_INPUT vsOut = (GSPS_INPUT)0;
	vsOut.Pos = float4(vertex.pos, 1.0);
	vsOut.Tex = vertex.tex;
	vsOut.Color = vertex.color;
	vsOut.Size = vertex.size;
	return vsOut;
}

