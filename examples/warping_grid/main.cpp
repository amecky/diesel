#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "WarpingGrid.h"
//#include "..\common\math.h"

struct CubeConstantBuffer {
	float viewProjectionMatrix[16];
	float worldMatrix[16];
};

RID vertexBufferID;


// ---------------------------------------------------------------
// initialize renderer
// ---------------------------------------------------------------
void initialize() {
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Geometry wars - Warping grid";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);
}

// ---------------------------------------------------------------
// create the stage group
// ---------------------------------------------------------------
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
	
	initialize();
	
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

	float viewMatrix[16];
	ds::matIdentity(viewMatrix);
	float projectionMatrix[16];
	ds::matOrthoLH(projectionMatrix, 1024.0f, 768.0f, 0.1f, 1.0f);
	float viewProjectionMatrix[16];
	ds::matMultiply(viewProjectionMatrix, viewMatrix, projectionMatrix);

	//ds::setViewMatrix(viewMatrix);
	//ds::setProjectionMatrix(projectionMatrix);

	CubeConstantBuffer constantBuffer;
	ds::matTranspose(constantBuffer.viewProjectionMatrix, viewProjectionMatrix);
	float worldMatrix[16];
	ds::matIdentity(worldMatrix);
	ds::matTranspose(constantBuffer.worldMatrix, worldMatrix);

	ds::StateGroup* stateGroup = createStateGroup(NUM, &constantBuffer);
	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST, 0 };

	ds::DrawItem* item = ds::compile(drawCmd, stateGroup);

	while (ds::isRunning()) {

		ds::begin();

		if (ds::isMouseButtonPressed(0)) {
			float mp[2];
			ds::getMousePosition(mp);
			grid.applyForce(v2(mp[0],mp[1]), 0.025f,80.0f,120.0f);
		}

		if (ds::isMouseButtonPressed(1)) {
			float mp[2];
			ds::getMousePosition(mp);
			grid.applyNegativeForce(v2(mp[0], mp[1]), 0.025f, 80.0f, 120.0f);
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