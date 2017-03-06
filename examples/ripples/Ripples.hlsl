cbuffer cbChangesPerObject : register( b0 )
{
    matrix mvp_;
    matrix world;
    float3 camera;
    float tmp;
    float3 light;    
    float more;
    float4 diffuseColor;
};


Texture2D colorMap_ : register( t0 );
SamplerState colorSampler_ : register( s0 );


struct VS_Input
{
    float4 pos  : POSITION;
    float3 normal : NORMAL;
    float2 tex0 : TEXCOORD0;
    float4 color : COLOR0;
};

struct PS_Input
{
    float4 pos  : SV_POSITION;
    float2 tex0 : TEXCOORD0;
    float4 color : COLOR0;
    float3 normal : NORMAL;
    float3 lightVec : TEXCOORD1;
    float3 viewVec : TEXCOORD2;
    float4 diffuseColor : COLOR1;
};


PS_Input VS_Main( VS_Input vertex )
{
    PS_Input vsOut = ( PS_Input )0;
    vsOut.pos = mul( vertex.pos, mvp_ );
    vsOut.tex0 = vertex.tex0;
    vsOut.color = vertex.color * diffuseColor;
    vsOut.normal = mul(vertex.normal,(float3x3)world);
    vsOut.normal = normalize(vsOut.normal);
    vsOut.lightVec = normalize(light);
    vsOut.viewVec = normalize(camera);
    vsOut.diffuseColor = diffuseColor;
    return vsOut;
}

float4 PS_Main_Plain( PS_Input frag ) : SV_TARGET
{
    return frag.color;
}

float4 PS_Main( PS_Input frag ) : SV_TARGET
{
    float4 ambientColor = float4(0.2,0.2,0.2,1.0);
    float4 textureColor = frag.color;
    float4 dc = float4(1,1,1,1);    
    float4 color = float4(0,0,0,0);    
    float3 n = normalize(frag.normal);
    float3 ln = normalize(frag.lightVec);
    float lightIntensity = saturate(dot(n,ln));    
    if ( lightIntensity > 0.0f ) {
         color += (dc * lightIntensity);   
         color.a = frag.color.a;        
    }
    else {
        color = ambientColor;  
    }     
    color = color * textureColor;
    color = saturate(color);    
    return color;
}


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