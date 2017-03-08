struct PS_Input {
    float4 pos  : SV_POSITION;
    float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};

float4 PS_Main( PS_Input frag ) : SV_TARGET {
	float2 t = frag.texcoord;
	float w = 0.02;
	float dw = 4.0 * w;
	if ((t.x < dw || t.x >(1 - dw)) && (t.y < dw || t.y >(1.0 - dw))) {
		return frag.color * 0.8;
	}
	if (t.x < w || t.x > (1 - w) || t.y < w || t.y > (1.0 - w)) {
		return frag.color;
	}
	else {
		return float4(0, 0, 0, 0);
	}
}
