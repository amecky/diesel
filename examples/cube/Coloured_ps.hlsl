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
