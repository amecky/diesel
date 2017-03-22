#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "WarpingGrid.h"

struct CubeConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
};

RID vertexBufferID;

RID createStateGroup(int numVertices,CubeConstantBuffer* buffer) {

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	RID vertexShader = ds::loadVertexShader("Grid_vs.cso");
	RID pixelShader = ds::loadPixelShader("Grid_ps.cso");

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	int q = numVertices / 4 * 6;

	RID rid = ds::createVertexDeclaration(decl, 3, vertexShader);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), buffer);
	RID indexBufferID = ds::createQuadIndexBuffer(numVertices / 4);
	vertexBufferID = ds::createVertexBuffer(ds::BufferType::DYNAMIC, numVertices, sizeof(GridVertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	

	RID stateGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.constantBuffer(cbid, vertexShader, 0)
		.vertexBuffer(vertexBufferID)
		.indexBuffer(indexBufferID)
		.build();

	return stateGroup;
	
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

	// create orthographic view
	ds::matrix orthoView = ds::matIdentity();
	ds::matrix orthoProjection = ds::matOrthoLH(ds::getScreenWidth(), ds::getScreenHeight(), 0.1f, 1.0f);
	RID orthoPass = ds::createRenderPass(orthoView, orthoProjection, ds::DepthBufferState::DISABLED);
	//constantBuffer.wvp = ds::matTranspose(orthoView * orthoProjection);

	CubeConstantBuffer constantBuffer;
	constantBuffer.viewprojectionMatrix = ds::matTranspose(orthoView * orthoProjection);
	constantBuffer.worldMatrix = ds::matTranspose(ds::matIdentity());

	RID stateGroup = createStateGroup(NUM, &constantBuffer);
	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST, 0 };

	RID item = ds::compile(drawCmd, stateGroup);

	

	while (ds::isRunning()) {

		ds::begin();

		if (ds::isMouseButtonPressed(0)) {
			grid.applyForce(ds::getMousePosition(), 0.025f,80.0f,120.0f);
		}

		if (ds::isMouseButtonPressed(1)) {
			grid.applyNegativeForce(ds::getMousePosition(), 0.025f, 80.0f, 120.0f);
		}

		grid.tick(static_cast<float>(ds::getElapsedSeconds()));

		//ds::setDepthBufferState(ds::DepthBufferState::DISABLED);

		int num = grid.mapData(vertices, NUM);

		ds::mapBufferData(vertexBufferID, vertices, num * sizeof(GridVertex));

		ds::submit(orthoPass, item, num / 4 * 6);

		//ds::setDepthBufferState(ds::DepthBufferState::ENABLED);

		ds::end();
	}

	delete[] vertices;

	ds::shutdown();
}