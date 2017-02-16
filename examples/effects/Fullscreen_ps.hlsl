struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD;
};

float4 PS_Main( PS_Input frag ) : SV_TARGET {
	float4 c = float4(1.0,1.0,1.0,1.0);
	c.r = (1.0 + frag.tex.x) * 0.5;
	c.g = (1.0 + frag.tex.y) * 0.5;
	return c;
}

