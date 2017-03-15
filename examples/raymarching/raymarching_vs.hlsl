struct PS_Input {
    float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD;
};

PS_Input VS_Main(uint id:SV_VERTEXID) {
	PS_Input vsOut;
	vsOut.pos.x = (float)(id / 2) * 4.0 - 1.0;
	vsOut.pos.y = (float)(id % 2) * 4.0 - 1.0;
	vsOut.pos.z = 0.0;
	vsOut.pos.w = 1.0;
	vsOut.tex.x = (float)(id / 2) * 2.0;
	vsOut.tex.y = 1.0 - (float)(id % 2) * 2.0;
    return vsOut;
}

