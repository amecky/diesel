cbuffer cbChangesPerFrame : register(b1) {
	float amplitude;
	float frequency;
	float ttl;
	float timer;
};


struct PS_Input {
    float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD;
};

PS_Input VS_Main(uint id:SV_VERTEXID) {
	float norm = clamp(0.0,1.0,timer / ttl);
	PS_Input vsOut;
	vsOut.pos.x = (float)(id / 2) * 4.0 - 1.0;
	vsOut.pos.y = (float)(id % 2) * 4.0 - 1.0;
	vsOut.pos.x += cos((norm * frequency) * 2 * 3.14159) * (1.0 - norm) * amplitude / 1024.0;
	vsOut.pos.y += sin((norm * frequency) * 2 * 3.14159) * (1.0 - norm) * amplitude / 768.0;
	vsOut.pos.z = 0.0;
	vsOut.pos.w = 1.0;
	vsOut.tex.x = (float)(id / 2) * 2.0;
	vsOut.tex.y = 1.0 - (float)(id % 2) * 2.0;
    return vsOut;
}

Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);

float4 PS_Main( PS_Input frag ) : SV_TARGET {
	return colorMap_.Sample(colorSampler_, frag.tex);
}

