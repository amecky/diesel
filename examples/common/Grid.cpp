#include "Grid.h"



void Grid::create(v3* positions, int numCells, RID shaderID, RID textureID) {
	RID blendState = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

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

	RID vertexDeclaration = ds::createVertexDeclaration(decl, 2, shaderID);
	RID bufferID = ds::createConstantBuffer(sizeof(GridConstantBuffer));
	RID indexBuffer = ds::createQuadIndexBuffer(1);
	RID gridBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, vertexDeclaration, _vertices, sizeof(GridVertex));
	RID samplerState = ds::createSamplerState(ds::TextureAddressModes::WRAP, ds::TextureFilters::LINEAR);

	_gridStates = ds::createStateGroup();
	_gridStates->bindLayout(vertexDeclaration);
	_gridStates->bindIndexBuffer(indexBuffer);
	_gridStates->bindVertexBuffer(gridBuffer);
	_gridStates->bindSamplerState(samplerState, ds::ShaderType::PIXEL);
	_gridStates->bindShader(shaderID);
	_gridStates->bindTexture(textureID, ds::ShaderType::PIXEL, 0);
	_gridStates->bindConstantBuffer(bufferID,ds::ShaderType::VERTEX, &_constantBuffer);
	drawCmd = { 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
}

void Grid::render() {
	_constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
	matrix world = mat_identity();
	_constantBuffer.worldMatrix = mat_Transpose(world);
	ds::submit(drawCmd, _gridStates);
}