cbuffer cbChangesPerObject : register( b0 ) {
    matrix mvp;
    matrix world;    
};

struct VS_Input {
    float4 position  : POSITION;
    float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4 color : COLOR0;
	matrix world : WORLD;
	float4 icolor : COLOR1;
};

struct PS_Input {
    float4 pos  : SV_POSITION;
	float3 normal : NORMAL;
    float4 color : COLOR;
};


PS_Input VS_Main( VS_Input vertex ) {
    PS_Input vsOut = ( PS_Input )0;
	vertex.position.w = 1.0;
	float4 p = vertex.position;
    vsOut.pos = mul( p, vertex.world);
	vsOut.pos = mul(vsOut.pos, mvp);
    vsOut.color = vertex.color * vertex.icolor;
	vsOut.normal = mul(vertex.normal, (float3x3)vertex.world);
    return vsOut;
}

cbuffer cbChangesPerObject : register(b0) {
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

float4 PS_Main( PS_Input frag ) : SV_TARGET {
	float4 textureColor = frag.color;
	float4 color = ambientColor;
	float3 lightDir = -lightDirection;
	float3 normal = normalize(frag.normal);
	float lightIntensity = saturate(dot(normal, lightDir));
	if (lightIntensity > 0.0f) {
		color += (diffuseColor * lightIntensity);
	}
	color = saturate(color);
	color = color * textureColor;
	color.a = textureColor.a;
	//float3 nv = float3(0.5,0.5,0.5) + frag.normal * 0.5;
	//color = float4(lightIntensity,lightIntensity,lightIntensity,1.0);
	//color = float4(nv,1.0);
	return color;
}

