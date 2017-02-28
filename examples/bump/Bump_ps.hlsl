struct PS_Input {
	float4 pos  : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
SamplerState colorSampler_ : register(s0);

float4 PS_Main( PS_Input frag ) : SV_TARGET {

	float3 lightDirection = float3(1,0,1);
	lightDirection = normalize(lightDirection);

	float4 diffuseColor = float4(1, 1, 1, 1);

	// Sample the texture pixel at this location.
	float4 textureColor = colorMap.Sample(colorSampler_, frag.texcoord);

	// Sample the pixel in the bump map.
	float4 bumpMap = normalMap .Sample(colorSampler_, frag.texcoord);

	// Expand the range of the normal value from (0, +1) to (-1, +1).
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	// Calculate the normal from the data in the bump map.
	float3 bumpNormal = (bumpMap.x * frag.tangent) + (bumpMap.y * frag.binormal) + (bumpMap.z * frag.normal);

	// Normalize the resulting bump normal.
	bumpNormal = normalize(bumpNormal);

	// Invert the light direction for calculations.
	float3 lightDir = -lightDirection;

	// Calculate the amount of light on this pixel based on the bump map normal value.
	float lightIntensity = saturate(dot(bumpNormal, lightDir));

	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
	float4 color = saturate(diffuseColor * lightIntensity);

	// Combine the final bump light color with the texture color.
	color = color * textureColor;
	color.a = textureColor.a;
	return bumpMap;
}

