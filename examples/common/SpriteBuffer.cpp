#include "SpriteBuffer.h"
#include "Sprite_VS_Main.inc"
#include "Sprite_GS_Main.inc"
#include "Sprite_PS_Main.inc"

SpriteBuffer::SpriteBuffer(int maxSprites, RID textureID) : _max(maxSprites) {

	_vertices = new SpriteBufferVertex[maxSprites];

	RID vertexShader = ds::createVertexShader(Sprite_VS_Main, sizeof(Sprite_VS_Main));
	_pixelShader = ds::createPixelShader(Sprite_PS_Main, sizeof(Sprite_PS_Main));
	RID geoShader = ds::createGeometryShader(Sprite_GS_Main, sizeof(Sprite_GS_Main));

	// very special buffer layout 
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	RID vertexDeclId = ds::createVertexDeclaration(decl, 4, vertexShader);

	RID cbid = ds::createConstantBuffer(sizeof(SpriteBufferConstantBuffer), &_constantBuffer);
	_vertexBufferID = ds::createVertexBuffer(ds::BufferType::DYNAMIC, maxSprites, sizeof(SpriteBufferVertex));

	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::POINT);
	// create orthographic view
	ds::matrix viewMatrix = ds::matIdentity();
	// FIXME: get screen size
	ds::matrix projectionMatrix = ds::matOrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	ds::matrix viewprojectionMatrix = viewMatrix * projectionMatrix;

	RID spriteStateGroup = ds::StateGroupBuilder()
		.inputLayout(vertexDeclId)
		.vertexBuffer(_vertexBufferID)
		.indexBuffer(NO_RID)
		.vertexShader(vertexShader)
		.geometryShader(geoShader)
		.pixelShader(_pixelShader)
		.constantBuffer(cbid, geoShader, 0)
		.texture(textureID, _pixelShader, 0)
		.samplerState(ssid, _pixelShader)
		.build();
	
	ds::vec2 textureSize = ds::getTextureSize(textureID);
	_constantBuffer.wvp = ds::matTranspose(viewprojectionMatrix);
	_constantBuffer.screenDimension = ds::vec4(1024.0f, 768.0f, textureSize.x, textureSize.y);

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::POINT_LIST, 0 };
	_item = ds::compile(drawCmd, spriteStateGroup);

	_current = 0;
}

void SpriteBuffer::begin() {
	_current = 0;
	//_currentTexture = NO_RID;
}

void SpriteBuffer::add(const ds::vec2& position, const ds::vec4& rect, const ds::vec2& scale, float rotation, const ds::Color& clr) {
	if ((_current + 1) >= _max) {
		flush();
	}
	_vertices[_current++] = SpriteBufferVertex(position, rect, ds::vec3(scale.x, scale.y, rotation), clr);
}

void SpriteBuffer::flush() {
	if (_current > 0) {
		ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
		ds::mapBufferData(_vertexBufferID, _vertices, _current * sizeof(SpriteBufferVertex));
		ds::submit(_item, _current);
		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
		_current = 0;
	}
}
