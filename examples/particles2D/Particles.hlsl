cbuffer cbChangesPerFrame : register(b0) {
	float4 screenCenter;
    float4 startColor;
	float4 endColor;
	float4 texRect;
    float2 dimension;
    float2 dummy;
	matrix wvp;
};

struct Particle {
	float2 position;
	float rotation;
	float2 velocity;
    float2 acceleration;
    float2 timer;
	float2 scale;
	float2 growth;
};

StructuredBuffer<Particle> ParticlesRO : register(t1);

struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex : TEXCOORD0;
	float4 color : COLOR0;
};

PS_Input VS_Main(uint id:SV_VERTEXID) {
	PS_Input vsOut = (PS_Input)0;
	uint particleIndex = id / 4;
	uint vertexIndex = id % 4;	
	float elapsed = ParticlesRO[particleIndex].timer.x;
    float norm = ParticlesRO[particleIndex].timer.y;	
	float3 position;
	position.x = (vertexIndex % 2) ? 0.5 : -0.5;
	position.y = (vertexIndex & 2) ? -0.5 : 0.5;
	position.z = 0.0;

	position.x *= dimension.x;
	position.y *= dimension.y;

	float rot = ParticlesRO[particleIndex].rotation;
	float s = sin(rot);
	float c = cos(rot);

    float2 scaling = ParticlesRO[particleIndex].scale;
	scaling += ParticlesRO[particleIndex].growth * elapsed;
	scaling = saturate(scaling);

	float sx = position.x * scaling.x;
	float sy = position.y * scaling.y;

	float xt = c * sx - s * sy;
	float yt = s * sx + c * sy;

	float3 sp = float3(ParticlesRO[particleIndex].position, 0.0);
    sp += float3(ParticlesRO[particleIndex].velocity,0.0) * elapsed;
    sp += float3(ParticlesRO[particleIndex].acceleration,0.0) * elapsed * elapsed;
	sp.x -= screenCenter.x;
	sp.y -= screenCenter.y;

	vsOut.pos = mul(float4(xt + sp.x, yt + sp.y, 0.0, 1.0f), wvp);
	vsOut.pos.z = 1.0;
	vsOut.tex.x = (vertexIndex % 2) ? texRect.x : texRect.z;
    vsOut.tex.y = (vertexIndex & 2) ? texRect.y : texRect.w;
	vsOut.color = lerp(startColor,endColor,norm);
	return vsOut;
}

Texture2D colorMap : register(t0);
SamplerState colorSampler : register(s0);

float4 PS_Main(PS_Input frag) : SV_TARGET{
	return colorMap.Sample(colorSampler, frag.tex) * frag.color;
}
