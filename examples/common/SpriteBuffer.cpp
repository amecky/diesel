#include "SpriteBuffer.h"
#include "Sprite_VS_Main.inc"
#include "Sprite_GS_Main.inc"
#include "Sprite_PS_Main.inc"



SpriteBuffer::SpriteBuffer(int maxSprites) : _max(maxSprites) {

	_vertices = new SpriteBufferVertex[maxSprites];

	ds::ShaderDataDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, Sprite_VS_Main,sizeof(Sprite_VS_Main) },
		{ ds::ShaderType::PIXEL, Sprite_PS_Main,sizeof(Sprite_PS_Main) },
		{ ds::ShaderType::GEOMETRY, Sprite_GS_Main,sizeof(Sprite_GS_Main) }
	};

	RID shaderID = ds::createShader(desc, 3);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	// very special buffer layout 
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	RID vertexDeclId = ds::createVertexDeclaration(decl, 4, shaderID);

	RID cbid = ds::createConstantBuffer(sizeof(SpriteBufferConstantBuffer));
	_vertexBufferID = ds::createVertexBuffer(ds::BufferType::DYNAMIC, maxSprites, sizeof(SpriteBufferVertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::POINT);

	// create orthographic view
	ds::matrix viewMatrix = ds::matIdentity();
	// FIXME: get screen size
	ds::matrix projectionMatrix = ds::matOrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	ds::matrix viewprojectionMatrix = viewMatrix * projectionMatrix;

	ds::setViewMatrix(viewMatrix);
	ds::setProjectionMatrix(projectionMatrix);
	_constantBuffer.wvp = ds::matTranspose(viewprojectionMatrix);
	_constantBuffer.screenDimension = ds::vec4(1024.0f, 768.0f, 1024.0f, 1024.f);
	_constantBuffer.screenCenter = ds::vec4(512.0f, 364.0f, 0.0f, 0.0f);

	ds::StateGroup* sg = ds::createStateGroup();
	sg->bindLayout(vertexDeclId);
	sg->bindShader(shaderID);
	sg->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &_constantBuffer);
	sg->bindConstantBuffer(cbid, ds::ShaderType::GEOMETRY, &_constantBuffer);
	sg->bindBlendState(bs_id);
	sg->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	sg->bindVertexBuffer(_vertexBufferID);

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::POINT_LIST, 0 };

	_current = 0;

	_item = ds::compile(drawCmd, sg);

}

void SpriteBuffer::begin() {
	_current = 0;
	_currentTexture = NO_RID;
}

void SpriteBuffer::add(const ds::vec2& position, RID textureID, const ds::vec4& rect, const ds::vec2& scale, float rotation, const ds::Color& clr) {
	if ((_current + 1) >= _max) {
		flush();
	}
	if (_currentTexture != textureID) {
		_currentTexture = textureID;
		flush();
	}
	_vertices[_current++] = SpriteBufferVertex(position, rect, ds::vec3(scale.x, scale.y, rotation), clr);
}

void SpriteBuffer::flush() {
	if (_current > 0) {
		_item->groups[0]->bindTexture(_currentTexture, ds::ShaderType::PIXEL, 0);
		// FIXME: get texture size and set it here
		_constantBuffer.screenDimension = ds::vec4(1024.0f, 768.0f, 1024.0f, 1024.f);
		ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
		ds::mapBufferData(_vertexBufferID, _vertices, _current * sizeof(SpriteBufferVertex));
		_item->command.size = _current;
		ds::submit(_item);
		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
		_current = 0;
	}
}
