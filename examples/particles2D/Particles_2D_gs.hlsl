cbuffer cbChangesPerFrame : register(b0) {
	float4 screenDimension;
	float4 screenCenter;
	matrix wvp;
};

struct GSPS_INPUT {
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Size : NORMAL;
	float4 Color : COLOR;
};

struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 tex0 : TEXCOORD0;
	float4 color : COLOR0;
};

[maxvertexcount(4)]
void GS_Main(point GSPS_INPUT gin[1], inout TriangleStream<PS_Input> triStream)
{
	float VP_ARRAY[8] = { -0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };
	float2 t[4];
	float2 dim = gin[0].Tex;
	t[0] = float2(0.0,0.0);
	t[1] = float2(1.0,0.0);
	t[2] = float2(0.0,1.0);
	t[3] = float2(1.0,1.0);
	float4 pos = gin[0].Pos;
	pos -= float4(screenCenter.x, screenCenter.y, 0.5, 1.0);
	PS_Input gout;
	triStream.RestartStrip();
	float rot = gin[0].Size.z;
	float s = sin(rot);
	float c = cos(rot);
	[unroll]
	for (int i = 0; i < 4; ++i) {
		float px = VP_ARRAY[i * 2] * dim.x;
		float py = VP_ARRAY[i * 2 + 1] * dim.y;
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

