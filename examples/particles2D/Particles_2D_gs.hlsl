cbuffer cbChangesPerFrame : register(b0) {
	float4 screenDimension;
	float4 screenCenter;
	float4 startColor;
	float4 endColor;
	float4 scale;
	float4 tex;
	matrix wvp;
};

struct GSPS_INPUT {
	float4 Pos : SV_POSITION;
	float4 Timer : COLOR;
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
	float2 dim = tex.zw;
	float left = tex.x / screenDimension.z;
	float top = tex.y / screenDimension.w;
	float width = tex.z / screenDimension.z;
	float height = tex.w / screenDimension.w;
	float right = left + width;
	float bottom = top + height;
	t[0] = float2(left, top);
	t[1] = float2(right, top);
	t[2] = float2(left, bottom);
	t[3] = float2(right, bottom);
	float4 pos = gin[0].Pos;
	float2 Size = float2(1, 1);
	Size.x = lerp(scale.x, scale.z, gin[0].Timer.z);
	Size.y = lerp(scale.y, scale.w, gin[0].Timer.z);
	float4 color = lerp(startColor, endColor, gin[0].Timer.z);
	pos -= float4(screenCenter.x, screenCenter.y, 0.5, 1.0);
	PS_Input gout;
	triStream.RestartStrip();
	float rot = gin[0].Timer.w;
	float s = sin(rot);
	float c = cos(rot);
	[unroll]
	for (int i = 0; i < 4; ++i) {
		float px = VP_ARRAY[i * 2] * dim.x;
		float py = VP_ARRAY[i * 2 + 1] * dim.y;
		float sx = px * Size.x;
		float sy = py * Size.y;
		float xt = c * sx - s * sy;
		float yt = s * sx + c * sy;
		gout.pos = mul(float4(xt + pos.x, yt + pos.y, 0.0f, 1.0f), wvp);
		gout.pos.z = 1.0;
		gout.tex0 = t[i];
		gout.color = color;
		triStream.Append(gout);
	}
	triStream.RestartStrip();
}

