#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "WarpingGrid.h"

struct CubeConstantBuffer {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
};

RID vertexBufferID;

ds::StateGroup* createStateGroup(int numVertices,CubeConstantBuffer* buffer) {

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "Grid_vs.cso" },
		{ ds::ShaderType::PIXEL, "Grid_ps.cso" }
	};

	RID shaderID = ds::createShader(desc, 2);

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	int q = numVertices / 4 * 6;

	RID rid = ds::createVertexDeclaration(decl, 3, shaderID);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID indexBufferID = ds::createQuadIndexBuffer(numVertices / 4);
	vertexBufferID = ds::createVertexBuffer(ds::BufferType::DYNAMIC, numVertices, sizeof(GridVertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	

	ds::StateGroup* sg = ds::createStateGroup();
	sg->bindLayout(rid);	
	sg->bindBlendState(bs_id);
	sg->bindShader(shaderID);
	sg->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, buffer);
	sg->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	sg->bindVertexBuffer(vertexBufferID);
	sg->bindIndexBuffer(indexBufferID);

	return sg;
	
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Geometry wars - Warping grid";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	
	WarpingGridData gridData;
	gridData.width = 41;
	gridData.height = 22;
	gridData.cellSize = 30.0f;
	gridData.flashTTL = 0.4f;
	gridData.regularColor = ds::Color(64, 64, 64, 255);
	gridData.flashColor = ds::Color(192, 0, 0, 255);

	WarpingGrid grid;
	grid.createGrid(gridData);

	const int NUM = gridData.width * gridData.height * 4;

	GridVertex* vertices = new GridVertex[NUM];

	matrix viewMatrix = mat_identity();
	matrix projectionMatrix = mat_OrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	matrix viewProjectionMatrix = viewMatrix * projectionMatrix;

	ds::setViewMatrix(viewMatrix);
	ds::setProjectionMatrix(projectionMatrix);

	CubeConstantBuffer constantBuffer;
	constantBuffer.viewProjectionMatrix = mat_Transpose(viewProjectionMatrix);
	constantBuffer.worldMatrix = mat_Transpose(mat_identity());

	ds::StateGroup* stateGroup = createStateGroup(NUM, &constantBuffer);
	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST, 0 };

	ds::DrawItem* item = ds::compile(drawCmd, stateGroup);

	

	while (ds::isRunning()) {

		ds::begin();

		if (ds::isMouseButtonPressed(0)) {
			grid.applyForce(ds::getMousePosition(), 0.025f,80.0f,120.0f);
		}

		if (ds::isMouseButtonPressed(1)) {
			grid.applyNegativeForce(ds::getMousePosition(), 0.025f, 80.0f, 120.0f);
		}

		grid.tick(static_cast<float>(ds::getElapsedSeconds()));

		ds::setDepthBufferState(ds::DepthBufferState::DISABLED);

		int num = grid.mapData(vertices, NUM);

		ds::mapBufferData(vertexBufferID, vertices, num * sizeof(GridVertex));

		item->command.size = num / 4 * 6;

		ds::submit(item);

		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);

		ds::end();
	}

	delete[] vertices;

	ds::shutdown();
}