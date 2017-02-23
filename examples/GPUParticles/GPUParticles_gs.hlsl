cbuffer cbChangesPerObject : register(b0) {
	float4 startColor;
	float4 endColor;
	matrix wvp;
	matrix world;
	float3 eyePos;
	float padding; 
};

struct GSPS_INPUT {
	float4 Pos : SV_POSITION;
	float3 Velocity : NORMAL;
	float2 Size : TEXCOORD;
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
	float2 dim = gin[0].Size;
	t[0] = float2(0.0,1.0);
	t[1] = float2(1.0,1.0);
	t[2] = float2(0.0,0.0);
	t[3] = float2(1.0,0.0);
	float4 pos = gin[0].Pos;


	float3 look = normalize(eyePos - pos.xyz);
	float3 right = normalize(cross(float3(0, 1, 0), look));
	float3 up = normalize(cross(look, right));

	float hw = 0.5 * gin[0].Size.x;
	float hh = 0.5 * gin[0].Size.y;
	PS_Input gout;
	triStream.RestartStrip();

	float4 v[4];

	v[0] = float4(gin[0].Pos + hw * right - hh * up, 1.0);
	v[1] = float4(gin[0].Pos + hw * right + hh * up, 1.0);
	v[2] = float4(gin[0].Pos - hw * right - hh * up, 1.0);
	v[3] = float4(gin[0].Pos - hw * right + hh * up, 1.0);

	[unroll]
	for (int i = 0; i < 4; ++i) {
		gout.pos = mul(v[i], wvp);
		gout.tex0 = t[i];
		gout.color = gin[0].Color;
		triStream.Append(gout);
	}
	triStream.RestartStrip();
}

