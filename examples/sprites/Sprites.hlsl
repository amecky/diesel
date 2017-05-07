cbuffer cbChangesPerFrame : register(b0) {
	float4 screenDimension;
	float4 screenCenter;
	matrix wvp;
};

struct Sprite {
	float2 position; // x, y world position
	float rotation;
	float scaling;
	float4 tex;
};

StructuredBuffer<Sprite> SpritesRO : register(t1);

struct GSPS_INPUT {
	float4 Pos : SV_POSITION;
	float4 Tex : COLOR0;
	float3 Size : NORMAL0;
	float4 Color : COLOR1;
};

struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float4 color : COLOR0;
};

PS_Input VS_Main(uint id:SV_VERTEXID) {
	PS_Input vsOut = (PS_Input)0;
	uint spriteIndex = id / 4;
	uint vertexIndex = id % 4;	
	float2 t[4];
	float4 ret = SpritesRO[spriteIndex].tex;
	float u1 = ret.x / screenDimension.z;
	float v1 = ret.y / screenDimension.w;
	float width = ret.z / screenDimension.z;
	float height = ret.w / screenDimension.w;
	float u2 = u1 + width;
	float v2 = v1 + height;
	t[0] = float2(u1, v1);
	t[1] = float2(u2, v1);
	t[2] = float2(u1, v2);
	t[3] = float2(u2, v2);

	float3 position;
	position.x = (vertexIndex % 2) ? 0.5 : -0.5;
	position.y = (vertexIndex & 2) ? -0.5 : 0.5;
	position.z = 0.0;

	position.x *= ret.z;
	position.y *= ret.w;

	float rot = SpritesRO[spriteIndex].rotation;
	float s = sin(rot);
	float c = cos(rot);

	float sx = position.x * SpritesRO[spriteIndex].scaling;
	float sy = position.y * SpritesRO[spriteIndex].scaling;

	float xt = c * sx - s * sy;
	float yt = s * sx + c * sy;

	float3 sp = float3(SpritesRO[spriteIndex].position, 0.0);

	sp.x -= screenCenter.x;
	sp.y -= screenCenter.y;

	vsOut.pos = mul(float4(xt + sp.x, yt + sp.y, 0.0, 1.0f), wvp);
	vsOut.pos.z = 1.0;
	vsOut.tex0 = t[vertexIndex];
	vsOut.color = float4(1.0,1.0,1.0,1.0);
	return vsOut;
}

Texture2D colorMap : register(t0);
SamplerState colorSampler : register(s0);

float4 PS_Main(PS_Input frag) : SV_TARGET{
	return colorMap.Sample(colorSampler, frag.tex0) * frag.color;
}
