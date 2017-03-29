#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
/*
	This demo draws a moving coloured cube.
*/

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {
	ds::vec3 p;
	ds::Color color;

	Vertex() : p(0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {}
	Vertex(const ds::vec3& pv, const ds::Color& c) : p(pv), color(c) {}
};

const ds::vec3 CUBE_VERTICES[8] = {
	ds::vec3(-0.5f,-0.5f,-0.5f),
	ds::vec3(-0.5f, 0.5f,-0.5f),
	ds::vec3( 0.5f, 0.5f,-0.5f),
	ds::vec3( 0.5f,-0.5f,-0.5f),
	ds::vec3(-0.5f,-0.5f, 0.5f),
	ds::vec3(-0.5f, 0.5f, 0.5f),
	ds::vec3( 0.5f, 0.5f, 0.5f),
	ds::vec3( 0.5f,-0.5f, 0.5f)
};

const int CUBE_PLANES[6][4] = {
	{ 0, 1, 2, 3 }, // front
	{ 3, 2, 6, 7 }, // left
	{ 1, 5, 6, 2 }, // top
	{ 4, 5, 1, 0 }, // right
	{ 4, 0, 3, 7 }, // bottom
	{ 7, 6, 5, 4 }, // back
};

// ---------------------------------------------------------------
// add plane
// ---------------------------------------------------------------
void addPlane(int side, const ds::Color& color, Vertex* vertices, uint32_t* indices) {
	int idx = side * 4;
	for (int i = 0; i < 4; ++i) {
		int p = idx + i;
		vertices[p] = Vertex(CUBE_VERTICES[CUBE_PLANES[side][i]], color);
	}
	int offset = side * 4;
	int ar[6] = { 0, 1, 2, 2, 3, 0 };
	for (int i = 0; i < 6; ++i) {
		indices[side * 6 + i] = offset + ar[i];
	}
}

// ---------------------------------------------------------------
// the cube constant buffer used by the vertex shader
// ---------------------------------------------------------------
struct CubeConstantBuffer {
	ds::matrix viewprojectionMatrix;
	ds::matrix worldMatrix;
};

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
int main(int argc, char *argv[]) {
	uint32_t p_indices[36];
	Vertex v[24];
	addPlane(0, ds::Color(1.0f, 0.0f, 0.0f, 1.0f), v, p_indices);
	addPlane(1, ds::Color(0.0f, 1.0f, 0.0f, 1.0f), v, p_indices);
	addPlane(2, ds::Color(0.0f, 0.0f, 1.0f, 1.0f), v, p_indices);
	addPlane(3, ds::Color(1.0f, 0.0f, 1.0f, 1.0f), v, p_indices);
	addPlane(4, ds::Color(1.0f, 1.0f, 0.0f, 1.0f), v, p_indices);
	addPlane(5, ds::Color(0.0f, 1.0f, 1.0f, 1.0f), v, p_indices);

	CubeConstantBuffer constantBuffer;
	float t = 0.0f;

	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Spinning cube";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	rs.supportDebug = true;
	ds::init(rs);

	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	RID vertexShader = ds::loadVertexShader("Cube_vs.cso");
	RID pixelShader = ds::loadPixelShader("Cube_ps.cso");

	ds::matrix viewMatrix = ds::matLookAtLH(ds::vec3(0.0f, 2.0f, -6.0f), ds::vec3(0, 0, 0), ds::vec3(0, 1, 0));
	ds::matrix projectionMatrix = ds::matPerspectiveFovLH(ds::PI / 4.0f, ds::getScreenAspectRatio(), 0.01f, 100.0f);
	RID basicPass = ds::createRenderPass(viewMatrix, projectionMatrix, ds::DepthBufferState::ENABLED);

	// create buffer input layout
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};
	RID rid = ds::createVertexDeclaration(decl, 2, vertexShader);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer), &constantBuffer);
	RID iid = ds::createIndexBuffer(36, ds::IndexType::UINT_32, ds::BufferType::STATIC, p_indices);
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), v);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	RID stateGroup = ds::StateGroupBuilder()
		.inputLayout(rid)
		.constantBuffer(cbid, vertexShader, 0)
		.blendState(bs_id)
		.samplerState(ssid, pixelShader)
		.vertexBuffer(vbid)
		.vertexShader(vertexShader)
		.pixelShader(pixelShader)
		.indexBuffer(iid)
		.build();

	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };
		
	RID drawItem = ds::compile(drawCmd, stateGroup);

	while (ds::isRunning()) {
		ds::begin();
		// move cube
		t += static_cast<float>(ds::getElapsedSeconds());		
		ds::vec3 cp = ds::vec3(cos(t), 0.0f, sin(t));
		ds::matrix bY = ds::matRotationY(t);
		float scale = 0.7f + sin(t) * 0.3f;
		ds::matrix s = ds::matScale(ds::vec3(scale));
		ds::matrix w = bY * s * ds::matTranslate(cp);
		
		constantBuffer.viewprojectionMatrix = ds::matTranspose(ds::getViewProjectionMatrix(basicPass));
		constantBuffer.worldMatrix = ds::matTranspose(w);

		ds::submit(basicPass, drawItem);

		ds::dbgPrint(0, 0, "FPS: %d", ds::getFramesPerSecond());
		ds::dbgPrint(0, 1, "Simple spinning cube demo");

		ds::end();
	}
	ds::shutdown();
}