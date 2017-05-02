cbuffer cbChangesPerFrame : register(b0) {
	float amplitude;
	float frequency;
	float ttl;
	float timer;
};

struct Pixel
{
	int colour;
};

StructuredBuffer<Pixel> Buffer0 : register(t0);
//RWStructuredBuffer<Pixel> BufferOut : register(u0);
RWTexture2D<float3> BufferOut : register(u0);

float3 readPixel(int x, int y)
{
	float3 output;
	uint index = (x + y * 1024);

	output.x = (float)(((Buffer0[index].colour) & 0x000000ff)) / 255.0f;
	output.y = (float)(((Buffer0[index].colour) & 0x0000ff00) >> 8) / 255.0f;
	output.z = (float)(((Buffer0[index].colour) & 0x00ff0000) >> 16) / 255.0f;

	return output;
}
/*
void writeToPixel(int x, int y, float3 colour)
{
	uint index = (x + y * 1024);

	int ired = (int)(clamp(colour.r, 0, 1) * 255);
	int igreen = (int)(clamp(colour.g, 0, 1) * 255) << 8;
	int iblue = (int)(clamp(colour.b, 0, 1) * 255) << 16;

	BufferOut[index] = ired + igreen + iblue;
}
*/
int convert(float3 colour)
{
	int ired = (int)(clamp(colour.r, 0, 1) * 255);
	int igreen = (int)(clamp(colour.g, 0, 1) * 255) << 8;
	int iblue = (int)(clamp(colour.b, 0, 1) * 255) << 16;

	return ired + igreen + iblue;
}

[numthreads(32, 16, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	float3 pixel = readPixel(dispatchThreadID.x, dispatchThreadID.y);
	//pixel.rgb = pixel.r * 0.3 + pixel.g * 0.59 + pixel.b * 0.11;
	float3 greyscale = dot(pixel.rgb, float3(0.30, 0.59, 0.11));
	float3 rgb = lerp(pixel.rgb, greyscale, timer);
	//p.colour = convert(pixel.rgb);
	BufferOut[dispatchThreadID.xy] = rgb;
	//writeToPixel(dispatchThreadID.x, dispatchThreadID.y, pixel);
}
