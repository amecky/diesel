cbuffer cbChangesPerObject : register( b0 ) {
    matrix mvp;
    matrix world;
    float4 ambientColor;
    float3 light;    
    float more;
    float4 diffuseColor;
};

struct VS_Input {
    float4 pos  : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

struct PS_Input {
    float4 pos  : SV_POSITION;
	float3 normal : NORMAL;
    float4 color : COLOR;
};


PS_Input VS_Main( VS_Input vertex ) {
    PS_Input vsOut = ( PS_Input )0;
	vertex.pos.w = 1.0;
    vsOut.pos = mul( vertex.pos, world);
	vsOut.pos = mul(vsOut.pos, mvp);
    vsOut.color = vertex.color;
	vsOut.normal = mul(vertex.normal, (float3x3)world);
	vsOut.normal = normalize(vsOut.normal);
    return vsOut;
}
/*
float4 PS_Main_Plain( PS_Input frag ) : SV_TARGET {
    return frag.color;
}
*/
float4 PS_Main( PS_Input frag ) : SV_TARGET {
    float4 textureColor = frag.color;
	float4 color = ambientColor;
	float3 lightDir = -light;
	float lightIntensity = saturate(dot(frag.normal, lightDir));
	if (lightIntensity > 0.0f) {
		color += (diffuseColor * lightIntensity);
	}
	color = saturate(color);
	color = color * textureColor;
	return color;
}

/*
float4 PS_Main_Specular( PS_Input frag ) : SV_TARGET
{
    float3 ambientColor = float3(0.3f,0.3f,0.3f);
    float4 lightColor = frag.color;
    ambientColor *= lightColor.rgb;
    float3 lightVec = normalize(frag.pos - frag.lightVec);
    //float3 lightVec = normalize(frag.lightVec);
    float3 normal = normalize(frag.normal);
    float diffuseTerm = clamp(dot(normal,lightVec),0.0f,1.0f);
    float specularTerm = 0.0f;
    if ( diffuseTerm > 0.0f ) {
        float3 viewVec = normalize(frag.viewVec);
        //float3 halfVec = normalize(lightVec + viewVec);
        //float3 halfVec = normalize(lightVec + viewVec);
        float3 halfVec = normalize(normalize(viewVec - frag.pos) - lightVec);
        specularTerm = pow(saturate(dot(normal,halfVec)),25);        
    }
    float3 finalColor = ambientColor +  lightColor.rgb * diffuseTerm + lightColor * specularTerm;
   	return float4(finalColor,lightColor.a);

}
*/