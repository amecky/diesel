StateGroupMapping

struct StageMapping {
    ResourceType resourceType;
    PipielineStage stage;
    ShaderType shaderType;
};

const static StageMapping STAGE_MAPPINGS[] = {
    { RT_TEXTURE, PLS_VS , ShaderType::Vertex},
    { RT_TEXTURE, PLS_PS , ShaderType::Pixel},
	{ RT_VERTEX_DECLARATION, PLS_IA, ShaderType::NO_Shader },
	{ RT_CONSTANT_BUFFER, PLS_VS, ShaderType::Vertex },
    { RT_CONSTANT_BUFFER, PLS_GS, ShaderType::Geometry },
    { RT_CONSTANT_BUFFER, PLS_PS, ShaderType::Pixel },
	{ RT_INDEX_BUFFER, PLS_IA, ShaderType::NO_Shader },
	{ RT_VERTEX_BUFFER, PLS_IA, ShaderType::NO_Shader },
	{ RT_SAMPLER_STATE, PLS_PS, ShaderType::Pixel },
	{ RT_BLENDSTATE, PLS_OM, ShaderType::NO_Shader }
	{ RT_SHADER, PLS_VS, ShaderType::Vertex },
    { RT_SHADER, PLS_GS, ShaderType::Geometry },
    { RT_SHADER, PLS_PS, ShaderType::Pixel },
	{ RT_SRV, PLS_PS, ShaderType::Pixel },
	{ RT_RASTERIZER_STATE, PLS_RS, ShaderType::NO_Shader }
	{ RT_INSTANCED_VERTEX_BUFFER, PLS_IA, ShaderType::NO_Shader }
};
// first 16 bit: index
// second 16 bit: stage

build mapping during compile!

uint32_t* mapping;
uint16_t numMappings;

addMapping(int index, RID rid, int shaderType) {
    uint16_t resourceType = type_mask(rid);
    uint16_t stage = getStage(rid,shaderType);
    uint32_t entry = index + ( stage << 16 );
    int idx = -1;
    for ( int i = 0; i < numMappings; ++i ) {
        uint16_t current = stage_mask(mappings[i]);
        if ( current > stage ) {
            idx = i;
        }
    }

    if ( idx != -1) {
        // insert here
        
    }
    else {
        // just append
        mappings[numMappins++] = entry;
    }
}