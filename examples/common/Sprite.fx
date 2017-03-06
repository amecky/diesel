cbuffer cbChangesPerFrame : register(b0) {
	float4 screenDimension;
	float4 screenCenter;
	matrix wvp;
};


Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);


struct VS_Input {
	float3 pos  : POSITION;
	float4 tex : COLOR0;
	float3 size : NORMAL0;
	float4 color : COLOR1;
};

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


GSPS_INPUT VS_Main(VS_Input vertex) {
	GSPS_INPUT vsOut = (GSPS_INPUT)0;
	vsOut.Pos = float4(vertex.pos, 1.0);
	vsOut.Tex = vertex.tex;
	vsOut.Color = vertex.color;
	vsOut.Size = vertex.size;
	return vsOut;
}

[maxvertexcount(4)]
void GS_Main(point GSPS_INPUT gin[1], inout TriangleStream<PS_Input> triStream)
{
	float VP_ARRAY[8] = { -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	float2 t[4];
	float4 ret = gin[0].Tex;
	float left = ret.x / screenDimension.z;
	float top = ret.y / screenDimension.w;
	float width = ret.z / screenDimension.z;
	float height = ret.w / screenDimension.w;
	float right = left + width;
	float bottom = top + height;
	t[0] = float2(left, top);
	t[1] = float2(right, top);
	t[2] = float2(left, bottom);
	t[3] = float2(right, bottom);


	float4 pos = gin[0].Pos;
	pos -= float4(screenCenter.x, screenCenter.y, 0.5, 1.0);
	PS_Input gout;
	triStream.RestartStrip();
	float rot = gin[0].Size.z;
	float s = sin(rot);
	float c = cos(rot);
	[unroll]
	for (int i = 0; i < 4; ++i) {
		float px = VP_ARRAY[i * 2] * ret.z;
		float py = VP_ARRAY[i * 2 + 1] * ret.w;
		float sx = px * gin[0].Size.x;
		float sy = py * gin[0].Size.y;
		float xt = c * sx - s * sy;
		float yt = s * sx + c * sy;
		gout.pos = mul(float4(xt + pos.x, yt + pos.y, 0.0f, 1.0f), wvp);
		gout.pos.z = 1.0;
		gout.tex0 = t[i];
		gout.color = gin[0].Color;
		triStream.Append(gout);
	}
	triStream.RestartStrip();
}


float4 PS_Main(PS_Input frag) : SV_TARGET{
	return colorMap_.Sample(colorSampler_, frag.tex0) * frag.color;
}