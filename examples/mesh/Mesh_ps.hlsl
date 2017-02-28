struct PS_Input {
	float4 pos  : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

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
