struct VS_Input {
	float3 pos  : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 color : COLOR;
};


struct GSPS_INPUT {
	float4 Pos : SV_POSITION;
	float4 Timer : COLOR;
};

GSPS_INPUT VS_Main(VS_Input vertex) {
	GSPS_INPUT vsOut = (GSPS_INPUT)0;
	vsOut.Pos = float4(vertex.pos, 1.0) + float4(vertex.normal,0.0) * vertex.color.x + float4(vertex.tangent,0.0) * vertex.color.x * vertex.color.x;
	vsOut.Timer = vertex.color;
	float norm = vertex.color.x / vertex.color.y;
	vsOut.Timer.z = norm;
	return vsOut;
}

