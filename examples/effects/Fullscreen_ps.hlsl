struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD;
};

float map(float3 p) {
	return length(p) - 1.0;
}

float trace(float3 o, float3 r) {
	float t = 0.0f;
	for (int i = 0; i < 32; ++i) {
		float3 p = o + r * t;
		float d = map(p);
		if (d < 0.001) {
			return 0.5f;
		}
		t += d * 0.5;
	}
	return 0.0;
}

float4 PS_Main( PS_Input frag ) : SV_TARGET {
	float2 p = frag.tex;
	p = p * 2.0 - 1.0;
	p.x *= 1024.0 / 768.0;
	float3 r = float3(p, 1.0);
	float3 o = float3(0, 0, -3);
	float d = trace(o, r);
	//float4 c = float4(frag.tex.x,frag.tex.y,0.5,1.0);
	float4 c = float4(d, d, d, 1.0);
	//c.r = (1.0 + frag.tex.x) * 0.5;
	//c.g = (1.0 + frag.tex.y) * 0.5;
	return c;
}

