#include "Grid.h"



void Grid::create(v3* positions, int numCells, RID shaderID, RID textureID) {
	_texture = textureID;
	_blendState = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);
	_shader = shaderID;

	float uvMax = static_cast<float>(numCells);
	v2 uvs[] = { v2(0.0f,uvMax),v2(0.0f,0.0f),v2(uvMax,0.0f),v2(uvMax,uvMax) };
	for (int i = 0; i < 4; ++i) {
		_vertices[i].p = positions[i];
		_vertices[i].uv = uvs[i];
	}

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	_vertexDeclaration = ds::createVertexDeclaration(decl, 2, _shader);
	_bufferID = ds::createConstantBuffer(sizeof(GridConstantBuffer));
	_indexBuffer = ds::createQuadIndexBuffer(1);
	_gridBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, _vertexDeclaration, _vertices, sizeof(GridVertex));
	_samplerState = ds::createSamplerState(ds::TextureAddressModes::WRAP, ds::TextureFilters::LINEAR);


	
}

void Grid::render(matrix* viewProjectionMatrix) {
	unsigned int stride = sizeof(GridVertex);
	unsigned int offset = 0;
	ds::setVertexDeclaration(_vertexDeclaration);
	ds::setIndexBuffer(_indexBuffer);
	ds::setBlendState(_blendState);
	ds::setShader(_shader);
	ds::setSamplerState(_samplerState);
	if (viewProjectionMatrix != 0) {
		_constantBuffer.viewProjectionMatrix = mat_Transpose(*viewProjectionMatrix);
	}
	else {
		_constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
	}
	matrix world = mat_identity();
	_constantBuffer.worldMatrix = mat_Transpose(world);
	ds::updateConstantBuffer(_bufferID, &_constantBuffer, sizeof(GridConstantBuffer));
	ds::setConstantBuffer(_bufferID, ds::ShaderType::VERTEX);
	ds::setTexture(_texture, ds::ShaderType::PIXEL);
	ds::setVertexBuffer(_gridBuffer, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
	ds::drawIndexed(6);
}