cbuffer cbChangesPerObject : register( b0 ) {
	float4 startColor;
	float4 endColor;
	matrix wvp;
	matrix world;
	float3 eyePos;
	float padding; 
};

struct VS_Input {
	float3 pos  : POSITION;
	float3 velocity : NORMAL;
	float2 timer : TEXCOORD;
	float4 size : COLOR;
};

struct GSPS_INPUT {
	float4 Pos : SV_POSITION;
	float3 Velocity : NORMAL;
	float2 Size : TEXCOORD;
	float4 Color : COLOR;
};

GSPS_INPUT VS_Main(VS_Input vertex) {
	GSPS_INPUT vsOut = (GSPS_INPUT)0;
	float3 p = vertex.pos + vertex.timer.x * vertex.velocity;
	vsOut.Pos = float4(p, 1.0);
	vsOut.Velocity = vertex.velocity;
	vsOut.Color = lerp(startColor,endColor,vertex.timer.y);
	vsOut.Color.a = 1.0 - vertex.timer.y;
	vsOut.Size = lerp(vertex.size.xy,vertex.size.zw,vertex.timer.y);
	return vsOut;
}

