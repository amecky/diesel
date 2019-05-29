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

float hash( float n ) {
    return frac(sin(n)*43758.5453);
}

float noise(float3 x) {
    // The noise function returns a value in the range -1.0f -> 1.0f

    float3 p = floor(x);
    float3 f = frac(x);

    f = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0 + 113.0*p.z;

    return lerp(lerp(lerp( hash(n+0.0), hash(n+1.0),f.x),
                   lerp( hash(n+57.0), hash(n+58.0),f.x),f.y),
               lerp(lerp( hash(n+113.0), hash(n+114.0),f.x),
                   lerp( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
}

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

    float3 vel = ParticlesRO[particleIndex].velocity;

    if ( pos.y < -1.0) {
		vel.y *= -1.0;
	}

    vel = float3(vel.x *  noise(1.3f * elapsed), 
				vel.y * (noise(elapsed + 21.5823f) * 0.5f + 0.5f),
				vel.z *  noise(0.6f * elapsed));

    pos += vel * elapsed;
    pos += ParticlesRO[particleIndex].acceleration * elapsed * elapsed;
	

	//ParticlesRO[particleIndex].velocity = vel;

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
