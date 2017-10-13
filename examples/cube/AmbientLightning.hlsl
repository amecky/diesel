cbuffer cbChangesPerObject : register( b0 ) {
    matrix mvp;
    matrix world;    
};

struct VS_Input {
    float4 position  : POSITION;
    float4 color : COLOR;
	float3 normal : NORMAL;
};

struct PS_Input {
    float4 pos  : SV_POSITION;
    float4 color : COLOR;
	float3 normal : NORMAL;
};


PS_Input VS_Main( VS_Input vertex ) {
    PS_Input vsOut = ( PS_Input )0;
	vertex.position.w = 1.0;
    vsOut.pos = mul( vertex.position, world);
	vsOut.pos = mul(vsOut.pos, mvp);
    vsOut.color = vertex.color;
	vsOut.normal = mul(vertex.normal, (float3x3)world);
	vsOut.normal = normalize(vsOut.normal);
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
	float lightIntensity = saturate(dot(frag.normal, lightDir));
	if (lightIntensity > 0.0f) {
		color += (diffuseColor * lightIntensity);
	}
	color = saturate(color);
	color = color * textureColor;
	return color;
}

