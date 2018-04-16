cbuffer cbChangesPerFrame : register(b0) {
	float4 startColor;
	float4 endColor;
	matrix wvp;
	matrix world;
	float3 eyePos;
	float padding; 
    float4 textureRect;
};

struct Particle {
	float3 position;
	float3 velocity;
    float3 acceleration;
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

float4 ComputePosition(float3 pos, float size, float rot, float2 vPos) {
	float3 toEye = normalize(eyePos - pos);
	float3 up = float3(0.0, 1.0, 0.0);
	float3 right = cross(toEye, up);
	up = cross(toEye, right);
	float s, c;
	sincos(rot, s, c); 
	float3 rightNew = c * right - s * up;
	float3 upNew = s * right + c * up;
	pos += (rightNew * size * vPos.x) - (upNew * size * vPos.y);
	return float4(pos,1.0);
}

PS_Input VS_Main(uint id:SV_VERTEXID) {
	PS_Input vsOut = (PS_Input)0;
	uint particleIndex = id / 4;
	uint vertexIndex = id % 4;	
	float elapsed = ParticlesRO[particleIndex].timer.x;
    float norm = ParticlesRO[particleIndex].timer.y;	
    float3 pos = ParticlesRO[particleIndex].position;

    pos += ParticlesRO[particleIndex].velocity * elapsed;
    pos += ParticlesRO[particleIndex].acceleration * elapsed * elapsed;
	if ( pos.y < -1.0) {
		pos.y = -1.0;
	}
    // move

    //float3 look = normalize(eyePos - pos);
	//float3 right = normalize(cross(float3(0, 1, 0), look));
	//float3 up = normalize(cross(look, right));

	float2 scaling = ParticlesRO[particleIndex].scale;
	scaling += ParticlesRO[particleIndex].growth * elapsed;
	scaling = saturate(scaling);
    
	float hw = (vertexIndex & 2) ? 0.5 : -0.5;
	float hh = (vertexIndex % 2) ? 0.5 : -0.5;
    hw *= scaling.x;
    hh *= scaling.y;

	//float4 fp = float4(pos + hw * right - hh * up, 1.0);
	float4 fp = ComputePosition(pos, 0.5, 0.0, float2(hw,hh));

    
	vsOut.pos = mul(fp, wvp);
	//vsOut.tex.x = (vertexIndex % 2) ? 1.0 : 0.0;
    //vsOut.tex.y = (vertexIndex & 2) ? 1.0 : 0.0;

    vsOut.tex.x = (vertexIndex % 2) ? textureRect.z : textureRect.x;
    vsOut.tex.y = (vertexIndex & 2) ? textureRect.w : textureRect.y;

	vsOut.color = lerp(startColor,endColor,norm);
	return vsOut;
}

Texture2D colorMap : register(t0);
SamplerState colorSampler : register(s0);

float4 PS_Main(PS_Input frag) : SV_TARGET{
	return colorMap.Sample(colorSampler, frag.tex) * frag.color;
	//return frag.color;
}
