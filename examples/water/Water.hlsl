cbuffer cbChangesPerObject : register( b0 ) {
    matrix mvp;    
	matrix world;    
};

cbuffer cbPixelData : register( b1 ) {  
    float4 data;
};

struct VS_Input {
    float4 position  : POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

struct PS_Input {
    float4 pos  : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};


PS_Input VS_Main( VS_Input vertex ) {
    PS_Input vsOut = ( PS_Input )0;
	vertex.position.w = 1.0;
    vsOut.pos = mul( vertex.position, world);
	vsOut.pos = mul(vsOut.pos, mvp);
    vsOut.color = vertex.color;
    vsOut.texcoord = vertex.texcoord;
    return vsOut;
}

Texture2D colorMap : register(t0);
Texture2D flowMap : register(t1);
SamplerState colorSampler : register(s0);

float3 FlowUV (float2 uv, float2 flowVector, float time) {
    float progress = frac(time);
    float3 ret;
	ret.xy = uv - flowVector * progress;
    ret.z = 1 - abs(1 - 2 * progress) * 0.5;
    return ret;
}

float4 PS_Main( PS_Input frag ) : SV_TARGET {    
    float2 flowVector = flowMap.Sample(colorSampler, frag.texcoord).rg * 2 - 1;
    float3 uv = FlowUV(frag.texcoord, flowVector, data.x);
    float4 c = colorMap.Sample(colorSampler, uv.xy) * uv.z;
    float blue = clamp(c.r+c.b+c.g,0,1);
    //return flowMap.Sample(colorSampler, uv);
    return float4(0,0,blue,1);
}
