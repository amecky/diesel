#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
/*
	This demo draws a moving coloured cube.
*/

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {
	float px;
	float py;
	float pz;
	ds::Color color;

	Vertex() : px(0.0f), py(0.0f), pz(0.0f), color(1.0f, 1.0f, 1.0f, 1.0f) {}
	Vertex(const float* pv, const ds::Color& c) : px(pv[0]), py(pv[1]), pz(pv[2]), color(c) {}
};

const float CUBE_VERTICES[8][3] = {
	{-0.5f,-0.5f,-0.5f },
	{ -0.5f, 0.5f,-0.5f },
	{ 0.5f, 0.5f,-0.5f },
	{ 0.5f,-0.5f,-0.5f },
	{ -0.5f,-0.5f, 0.5f },
	{ -0.5f, 0.5f, 0.5f },
	{ 0.5f, 0.5f, 0.5f },
	{ 0.5f,-0.5f, 0.5f }
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
		vertices[p].px = CUBE_VERTICES[CUBE_PLANES[side][i]][0];
		vertices[p].py = CUBE_VERTICES[CUBE_PLANES[side][i]][1];
		vertices[p].pz = CUBE_VERTICES[CUBE_PLANES[side][i]][2];
		vertices[p].color = color;
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
	float viewProjectionMatrix[16];
	float worldMatrix[16];
};

void initialize() {
	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	initialize();

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



	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "Cube_vs.cso" },
		{ ds::ShaderType::PIXEL, "Cube_ps.cso" }
	};

	RID shaderID = ds::createShader(desc, 2);

	// create buffer input layout
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};
	RID rid = ds::createVertexDeclaration(decl, 2, shaderID);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID iid = ds::createIndexBuffer(36, ds::IndexType::UINT_32, ds::BufferType::STATIC, p_indices);
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, 24, sizeof(Vertex), v);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	float vp[3] = { 0.0f, 2.0f, -6.0f };
	ds::setViewPosition(vp);

	float vpm[16];
	ds::getViewProjectionMatrix(vpm);
	ds::matTranspose(constantBuffer.viewProjectionMatrix, vpm);

	ds::StateGroup* sg = ds::createStateGroup();
	sg->bindLayout(rid);
	sg->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &constantBuffer);
	sg->bindBlendState(bs_id);
	sg->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	sg->bindVertexBuffer(vbid);
	sg->bindShader(shaderID);
	sg->bindIndexBuffer(iid);
	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	float world[16];
	ds::matIdentity(world);

	float matScale[16];
	float vecScale[3];

	while (ds::isRunning()) {
		ds::begin();
		// move cube
		t += static_cast<float>(ds::getElapsedSeconds());		
		float scale = 0.7f + sin(t) * 0.3f;
		ds::matSRT(world, scale, scale, scale, 0.0f, t, 0.0f, cos(t), 0.0f, sin(t));
		ds::matTranspose(constantBuffer.worldMatrix, world);

		ds::submit(drawCmd, sg);

		ds::end();
	}
	ds::shutdown();
}