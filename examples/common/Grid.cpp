#include "Grid.h"



void Grid::create(ds::vec3* positions, int numCells, RID vertexShader, RID pixelShader, RID textureID, RID renderPass) {
	_renderPass = renderPass;
	float uvMax = static_cast<float>(numCells);
	ds::vec2 uvs[] = { ds::vec2(0.0f,uvMax),ds::vec2(0.0f,0.0f),ds::vec2(uvMax,0.0f),ds::vec2(uvMax,uvMax) };
	for (int i = 0; i < 4; ++i) {
		_vertices[i].p = positions[i];
		_vertices[i].uv = uvs[i];
	}

	ds::InputLayoutDefinition decl[] = {
		{ "POSITION", 0, ds::BufferAttributeType::FLOAT3 },
		{ "TEXCOORD", 0, ds::BufferAttributeType::FLOAT2 }
	};
	
	RID vertexDeclaration = ds::createInputLayout(ds::InputLayoutDesc()
		.Declarations(decl)
		.NumDeclarations(2)
		.VertexShader(vertexShader)
		, "PT_Layout"
	);

	RID bufferID = ds::createConstantBuffer(sizeof(GridConstantBuffer), &_constantBuffer, "GridConstantBuffer");
	RID indexBuffer = ds::createQuadIndexBuffer(1, "GridIndexBuffer");
	ds::VertexBufferInfo vbInfo = { ds::BufferType::STATIC, 4, sizeof(GridVertex), _vertices };
	RID gridBuffer = ds::createVertexBuffer(vbInfo, "GridVertexBuffer");
	
	RID samplerState = ds::createSamplerState(ds::SamplerStateDesc()
		.AddressMode(ds::TextureAddressModes::WRAP)
		.Filter(ds::TextureFilters::LINEAR)
	);

	RID gridStates = ds::StateGroupBuilder()
		.inputLayout(vertexDeclaration)
		.indexBuffer(indexBuffer)
		.vertexBuffer(gridBuffer)
		.samplerState(samplerState, pixelShader)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.texture(textureID, pixelShader, 0)
		.constantBuffer(bufferID, vertexShader, 0)
		.build();

	ds::DrawCommand drawCmd = { 6, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	_drawItem = ds::compile(drawCmd, gridStates, "Grid");
}

void Grid::render() {
	_constantBuffer.viewprojectionMatrix = ds::matTranspose(_camera->viewProjectionMatrix);
	ds::matrix world = ds::matIdentity();
	_constantBuffer.worldMatrix = ds::matTranspose(world);
	ds::submit(_renderPass, _drawItem);
}