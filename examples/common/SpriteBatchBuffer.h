#pragma once
#include "..\..\diesel.h"
//#define SPRITE_IMPLEMENTATION

struct SpriteBatchConstantBuffer {
	ds::vec4 screenCenter;
	ds::matrix wvp;
};

struct Sprite {
	ds::vec2 position;
	ds::vec4 textureRect;
	ds::vec2 scaling;
	float rotation;
	ds::Color color;
};

struct SpriteBatchBufferInfo {

	unsigned int maxSprites;
	RID textureID;
	ds::TextureFilters textureFilter;
	RID blendState;

	SpriteBatchBufferInfo() : maxSprites(1024), textureID(NO_RID), textureFilter(ds::TextureFilters::LINEAR) , blendState(NO_RID) {}
	SpriteBatchBufferInfo(int max,RID tex) : maxSprites(max), textureID(tex), textureFilter(ds::TextureFilters::LINEAR), blendState(NO_RID) {}
	SpriteBatchBufferInfo(int max, RID tex, ds::TextureFilters filter) : maxSprites(max), textureID(tex), textureFilter(filter), blendState(NO_RID) {}
	SpriteBatchBufferInfo(int max, RID tex, ds::TextureFilters filter,RID blend) : maxSprites(max), textureID(tex), textureFilter(filter), blendState(blend) {}
};

class SpriteBatchBuffer {

public:
	SpriteBatchBuffer(const SpriteBatchBufferInfo& info);
	~SpriteBatchBuffer();
	void begin();
	void add(const ds::vec2& pos, const ds::vec4& textureRect, const ds::vec2& scaling = ds::vec2(1, 1), float rotation = 0.0f, const ds::Color& color = ds::Color(255, 255, 255, 255));
	void add(const Sprite& sprite);
	void flush();
private:
	unsigned int _max;
	unsigned int _current;
	RID _drawItem;
	RID _renderPass;
	Sprite* _buffer;
	RID _structuredBufferId;
	SpriteBatchConstantBuffer _constantBuffer;
};

#ifdef SPRITE_IMPLEMENTATION

const BYTE Sprites_VS_Main[] =
{
	68,  88,  66,  67, 176, 179,
	62,  76, 198,  29, 218, 165,
	231, 174, 221,  37,  99, 133,
	84, 135,   1,   0,   0,   0,
	228,   4,   0,   0,   4,   0,
	0,   0,  48,   0,   0,   0,
	100,   0,   0,   0, 216,   0,
	0,   0, 212,   4,   0,   0,
	73,  83,  71,  78,  44,   0,
	0,   0,   1,   0,   0,   0,
	8,   0,   0,   0,  32,   0,
	0,   0,   0,   0,   0,   0,
	6,   0,   0,   0,   1,   0,
	0,   0,   0,   0,   0,   0,
	1,   1,   0,   0,  83,  86,
	95,  86,  69,  82,  84,  69,
	88,  73,  68,   0,  79,  83,
	71,  78, 108,   0,   0,   0,
	3,   0,   0,   0,   8,   0,
	0,   0,  80,   0,   0,   0,
	0,   0,   0,   0,   1,   0,
	0,   0,   3,   0,   0,   0,
	0,   0,   0,   0,  15,   0,
	0,   0,  92,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   3,   0,   0,   0,
	1,   0,   0,   0,   3,  12,
	0,   0, 101,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   3,   0,   0,   0,
	2,   0,   0,   0,  15,   0,
	0,   0,  83,  86,  95,  80,
	79,  83,  73,  84,  73,  79,
	78,   0,  84,  69,  88,  67,
	79,  79,  82,  68,   0,  67,
	79,  76,  79,  82,   0, 171,
	83,  72,  69,  88, 244,   3,
	0,   0,  64,   0,   1,   0,
	253,   0,   0,   0, 106,  72,
	0,   1,  89,   0,   0,   4,
	70, 142,  32,   0,   0,   0,
	0,   0,   5,   0,   0,   0,
	162,   0,   0,   4,   0, 112,
	16,   0,   1,   0,   0,   0,
	52,   0,   0,   0,  96,   0,
	0,   4,  18,  16,  16,   0,
	0,   0,   0,   0,   6,   0,
	0,   0, 103,   0,   0,   4,
	242,  32,  16,   0,   0,   0,
	0,   0,   1,   0,   0,   0,
	101,   0,   0,   3,  50,  32,
	16,   0,   1,   0,   0,   0,
	101,   0,   0,   3, 242,  32,
	16,   0,   2,   0,   0,   0,
	104,   0,   0,   2,   4,   0,
	0,   0, 105,   0,   0,   4,
	0,   0,   0,   0,   4,   0,
	0,   0,   4,   0,   0,   0,
	85,   0,   0,   7,  18,   0,
	16,   0,   0,   0,   0,   0,
	10,  16,  16,   0,   0,   0,
	0,   0,   1,  64,   0,   0,
	2,   0,   0,   0, 167,   0,
	0,   9, 242,   0,  16,   0,
	1,   0,   0,   0,  10,   0,
	16,   0,   0,   0,   0,   0,
	1,  64,   0,   0,   8,   0,
	0,   0,  70, 126,  16,   0,
	1,   0,   0,   0,  14,   0,
	0,   8, 242,   0,  16,   0,
	2,   0,   0,   0,  70,  14,
	16,   0,   1,   0,   0,   0,
	230, 142,  32,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	54,   0,   0,   6,  50,  48,
	32,   0,   0,   0,   0,   0,
	0,   0,   0,   0,  70,   0,
	16,   0,   2,   0,   0,   0,
	54,   0,   0,   5, 194,   0,
	16,   0,   3,   0,   0,   0,
	86,   1,  16,   0,   2,   0,
	0,   0,   0,   0,   0,   7,
	50,   0,  16,   0,   3,   0,
	0,   0, 230,  10,  16,   0,
	2,   0,   0,   0,  70,   0,
	16,   0,   2,   0,   0,   0,
	54,   0,   0,   6,  50,  48,
	32,   0,   0,   0,   0,   0,
	1,   0,   0,   0, 134,   0,
	16,   0,   3,   0,   0,   0,
	54,   0,   0,   6,  50,  48,
	32,   0,   0,   0,   0,   0,
	2,   0,   0,   0, 118,  15,
	16,   0,   3,   0,   0,   0,
	54,   0,   0,   6,  50,  48,
	32,   0,   0,   0,   0,   0,
	3,   0,   0,   0,  70,   0,
	16,   0,   3,   0,   0,   0,
	1,   0,   0,  10, 226,   0,
	16,   0,   0,   0,   0,   0,
	6,  16,  16,   0,   0,   0,
	0,   0,   2,  64,   0,   0,
	0,   0,   0,   0,   3,   0,
	0,   0,   1,   0,   0,   0,
	2,   0,   0,   0,  55,   0,
	0,  15, 194,   0,  16,   0,
	0,   0,   0,   0, 246,  11,
	16,   0,   0,   0,   0,   0,
	2,  64,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0, 191,   0,   0,
	0,  63,   2,  64,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,  63,
	0,   0,   0, 191,  54,   0,
	0,   7,  50,  32,  16,   0,
	1,   0,   0,   0,  70,  48,
	32,   4,   0,   0,   0,   0,
	26,   0,  16,   0,   0,   0,
	0,   0,  56,   0,   0,   7,
	98,   0,  16,   0,   0,   0,
	0,   0, 246,  14,  16,   0,
	1,   0,   0,   0, 166,  11,
	16,   0,   0,   0,   0,   0,
	167,   0,   0,   9, 242,   0,
	16,   0,   1,   0,   0,   0,
	10,   0,  16,   0,   0,   0,
	0,   0,   1,  64,   0,   0,
	24,   0,   0,   0,  70, 126,
	16,   0,   1,   0,   0,   0,
	56,   0,   0,   7,  98,   0,
	16,   0,   0,   0,   0,   0,
	86,   6,  16,   0,   0,   0,
	0,   0,  86,   4,  16,   0,
	1,   0,   0,   0,  77,   0,
	0,   7,  18,   0,  16,   0,
	2,   0,   0,   0,  18,   0,
	16,   0,   3,   0,   0,   0,
	42,   0,  16,   0,   1,   0,
	0,   0,  56,   0,   0,   7,
	50,   0,  16,   0,   2,   0,
	0,   0, 150,   5,  16,   0,
	0,   0,   0,   0,   6,   0,
	16,   0,   2,   0,   0,   0,
	50,   0,   0,  10,  66,   0,
	16,   0,   0,   0,   0,   0,
	10,   0,  16,   0,   3,   0,
	0,   0,  42,   0,  16,   0,
	0,   0,   0,   0,  10,   0,
	16, 128,  65,   0,   0,   0,
	2,   0,   0,   0,  50,   0,
	0,   9,  34,   0,  16,   0,
	0,   0,   0,   0,  10,   0,
	16,   0,   3,   0,   0,   0,
	26,   0,  16,   0,   0,   0,
	0,   0,  26,   0,  16,   0,
	2,   0,   0,   0, 167,   0,
	0,   9,  50,   0,  16,   0,
	2,   0,   0,   0,  10,   0,
	16,   0,   0,   0,   0,   0,
	1,  64,   0,   0,   0,   0,
	0,   0,  70, 112,  16,   0,
	1,   0,   0,   0, 167,   0,
	0,   9, 114,   0,  16,   0,
	1,   0,   0,   0,  10,   0,
	16,   0,   0,   0,   0,   0,
	1,  64,   0,   0,  40,   0,
	0,   0,  70, 114,  16,   0,
	1,   0,   0,   0,  54,   0,
	0,   5, 242,  32,  16,   0,
	2,   0,   0,   0,  54,   9,
	16,   0,   1,   0,   0,   0,
	0,   0,   0,   9, 146,   0,
	16,   0,   0,   0,   0,   0,
	6,   4,  16,   0,   2,   0,
	0,   0,   6, 132,  32, 128,
	65,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   7,  50,   0,
	16,   0,   1,   0,   0,   0,
	198,   0,  16,   0,   0,   0,
	0,   0, 102,  10,  16,   0,
	0,   0,   0,   0,  54,   0,
	0,   5,  66,   0,  16,   0,
	1,   0,   0,   0,   1,  64,
	0,   0,   0,   0, 128,  63,
	16,   0,   0,   8,  18,  32,
	16,   0,   0,   0,   0,   0,
	70,   2,  16,   0,   1,   0,
	0,   0,  70, 131,  32,   0,
	0,   0,   0,   0,   1,   0,
	0,   0,  16,   0,   0,   8,
	34,  32,  16,   0,   0,   0,
	0,   0,  70,   2,  16,   0,
	1,   0,   0,   0,  70, 131,
	32,   0,   0,   0,   0,   0,
	2,   0,   0,   0,  16,   0,
	0,   8, 130,  32,  16,   0,
	0,   0,   0,   0,  70,   2,
	16,   0,   1,   0,   0,   0,
	70, 131,  32,   0,   0,   0,
	0,   0,   4,   0,   0,   0,
	54,   0,   0,   5,  66,  32,
	16,   0,   0,   0,   0,   0,
	1,  64,   0,   0,   0,   0,
	128,  63,  62,   0,   0,   1,
	83,  70,  73,  48,   8,   0,
	0,   0,   2,   0,   0,   0,
	0,   0,   0,   0
};


const BYTE Sprites_PS_Main[] =
{
	68,  88,  66,  67, 128, 110,
	6,  17, 183, 249, 240,  47,
	66,  79,  68, 157, 198, 184,
	192,  10,   1,   0,   0,   0,
	160,   1,   0,   0,   3,   0,
	0,   0,  44,   0,   0,   0,
	160,   0,   0,   0, 212,   0,
	0,   0,  73,  83,  71,  78,
	108,   0,   0,   0,   3,   0,
	0,   0,   8,   0,   0,   0,
	80,   0,   0,   0,   0,   0,
	0,   0,   1,   0,   0,   0,
	3,   0,   0,   0,   0,   0,
	0,   0,  15,   0,   0,   0,
	92,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	3,   0,   0,   0,   1,   0,
	0,   0,   3,   3,   0,   0,
	101,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	3,   0,   0,   0,   2,   0,
	0,   0,  15,  15,   0,   0,
	83,  86,  95,  80,  79,  83,
	73,  84,  73,  79,  78,   0,
	84,  69,  88,  67,  79,  79,
	82,  68,   0,  67,  79,  76,
	79,  82,   0, 171,  79,  83,
	71,  78,  44,   0,   0,   0,
	1,   0,   0,   0,   8,   0,
	0,   0,  32,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   3,   0,   0,   0,
	0,   0,   0,   0,  15,   0,
	0,   0,  83,  86,  95,  84,
	65,  82,  71,  69,  84,   0,
	171, 171,  83,  72,  68,  82,
	196,   0,   0,   0,  64,   0,
	0,   0,  49,   0,   0,   0,
	90,   0,   0,   3,   0,  96,
	16,   0,   0,   0,   0,   0,
	88,  24,   0,   4,   0, 112,
	16,   0,   0,   0,   0,   0,
	85,  85,   0,   0,  98,  16,
	0,   3,  50,  16,  16,   0,
	1,   0,   0,   0,  98,  16,
	0,   3, 242,  16,  16,   0,
	2,   0,   0,   0, 101,   0,
	0,   3, 242,  32,  16,   0,
	0,   0,   0,   0, 104,   0,
	0,   2,   1,   0,   0,   0,
	69,   0,   0,   9, 242,   0,
	16,   0,   0,   0,   0,   0,
	70,  16,  16,   0,   1,   0,
	0,   0,  70, 126,  16,   0,
	0,   0,   0,   0,   0,  96,
	16,   0,   0,   0,   0,   0,
	56,   0,   0,   7, 242,   0,
	16,   0,   0,   0,   0,   0,
	70,  14,  16,   0,   0,   0,
	0,   0,  70,  30,  16,   0,
	2,   0,   0,   0,  56,   0,
	0,   7, 114,  32,  16,   0,
	0,   0,   0,   0, 246,  15,
	16,   0,   0,   0,   0,   0,
	70,   2,  16,   0,   0,   0,
	0,   0,  54,   0,   0,   5,
	130,  32,  16,   0,   0,   0,
	0,   0,  58,   0,  16,   0,
	0,   0,   0,   0,  62,   0,
	0,   1
};


SpriteBatchBuffer::SpriteBatchBuffer(const SpriteBatchBufferInfo& info) : _max(info.maxSprites) , _current(0) {

	_buffer = new Sprite[_max];
	ds::vec2 textureSize = ds::getTextureSize(info.textureID);
	_constantBuffer.screenCenter = { static_cast<float>(ds::getScreenWidth()) / 2.0f, static_cast<float>(ds::getScreenHeight()) / 2.0f, textureSize.x, textureSize.y };

	ds::ShaderInfo vsInfo = { 0 , Sprites_VS_Main, sizeof(Sprites_VS_Main), ds::ShaderType::ST_VERTEX_SHADER };
	RID vertexShader = ds::createShader(vsInfo, "SpritesVS");
	ds::ShaderInfo psInfo = { 0 , Sprites_PS_Main, sizeof(Sprites_PS_Main), ds::ShaderType::ST_PIXEL_SHADER };
	RID pixelShader = ds::createShader(psInfo, "SpritesPS");

	RID bs_id = info.blendState;
	if (bs_id == NO_RID) {
		ds::BlendStateInfo blendInfo = { ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true };
		bs_id = ds::createBlendState(blendInfo);
	}
	RID constantBuffer = ds::createConstantBuffer(sizeof(SpriteBatchConstantBuffer), &_constantBuffer);

	ds::SamplerStateInfo samplerInfo = { ds::TextureAddressModes::CLAMP, info.textureFilter };
	RID ssid = ds::createSamplerState(samplerInfo);

	int indices[] = { 0,1,2,1,3,2 };
	RID idxBuffer = ds::createQuadIndexBuffer(_max, indices);

	ds::StructuredBufferInfo sbInfo;
	sbInfo.cpuWritable = true;
	sbInfo.data = 0;
	sbInfo.elementSize = sizeof(Sprite);
	sbInfo.numElements = _max;
	sbInfo.gpuWritable = false;
	sbInfo.renderTarget = NO_RID;
	sbInfo.textureID = NO_RID;
	_structuredBufferId = ds::createStructuredBuffer(sbInfo);

	RID basicGroup = ds::StateGroupBuilder()
		.constantBuffer(constantBuffer, vertexShader)
		.blendState(bs_id)
		.structuredBuffer(_structuredBufferId, vertexShader, 1)
		.vertexBuffer(NO_RID)
		.vertexShader(vertexShader)
		.indexBuffer(idxBuffer)
		.pixelShader(pixelShader)
		.samplerState(ssid, pixelShader)
		.texture(info.textureID, pixelShader, 0)
		.build();

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	_drawItem = ds::compile(drawCmd, basicGroup, "SpritesDrawItem");

	// create orthographic view
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(ds::getScreenWidth(), ds::getScreenHeight(), 0.0f, 1.0f);
	ds::Camera camera = {
		orthoView,
		orthoProjection,
		orthoView * orthoProjection,
		ds::vec3(0,0,0),
		ds::vec3(0,0,1),
		ds::vec3(0,1,0),
		ds::vec3(1,0,0),
		0.0f,
		0.0f,
		0.0f
	};
	ds::ViewportInfo vpInfo = { ds::getScreenWidth(), ds::getScreenHeight(), 0.0f, 1.0f };
	RID vp = ds::createViewport(vpInfo, "SpriteOrthoViewport");
	ds::RenderPassInfo rpInfo = { &camera, vp, ds::DepthBufferState::DISABLED, 0, 0 };
	_renderPass = ds::createRenderPass(rpInfo, "SpritesOrthoPass");
	_constantBuffer.wvp = ds::matTranspose(camera.viewProjectionMatrix);
}

SpriteBatchBuffer::~SpriteBatchBuffer() {
	delete[] _buffer;
}

void SpriteBatchBuffer::begin() {
	_current = 0;
}

void SpriteBatchBuffer::add(const ds::vec2& position, const ds::vec4& rect, const ds::vec2& scale, float rotation, const ds::Color& clr) {
	if ((_current + 1) >= _max) {
		flush();
	}
	_buffer[_current++] = { position, rect, scale, rotation, clr };
}

void SpriteBatchBuffer::add(const Sprite& sprite) {
	if ((_current + 1) >= _max) {
		flush();
	}
	_buffer[_current++] = sprite;
}

void SpriteBatchBuffer::flush() {
	if (_current > 0) {
		ds::mapBufferData(_structuredBufferId, _buffer, _current * sizeof(Sprite));
		ds::submit(_renderPass, _drawItem, _current * 6);
		_current = 0;
	}
}


#endif
