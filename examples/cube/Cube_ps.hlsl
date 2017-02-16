struct PS_Input {
    float4 pos  : SV_POSITION;
    float4 color : COLOR;
};

float4 PS_Main( PS_Input frag ) : SV_TARGET {
    return frag.color;
}
