#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "..\common\Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\Grid.h"
#include "..\common\Geometry.h"

// ---------------------------------------------------------------
// Vertex
// ---------------------------------------------------------------
struct Vertex {

	v3 p;
	v2 uv;

	Vertex() : p(0.0f), uv(0.0f) {}
	Vertex(const v3& pv, float u, float v) : p(pv), uv(u, v) {}
	Vertex(const v3& pv, const v2& uvv) : p(pv), uv(uvv) {}
};

struct CubeConstantBuffer {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
};

// ---------------------------------------------------------------
// load image using stb_image
// ---------------------------------------------------------------
RID loadImage(const char* name) {
	int x, y, n;
	unsigned char *data = stbi_load(name, &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);
	return textureID;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {


	v3 positions[24];
	v2 uvs[24];
	matrix m = mat_identity();
	geometry::buildCube(m, positions, uvs);

	Vertex v[24];
	for (int i = 0; i < 24; ++i) {
		v[i] = Vertex(positions[i], uvs[i]);
	}

	CubeConstantBuffer constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Camera Demo";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 1;
	ds::init(rs);

	RID textureID = loadImage("..\\common\\cube_map.png");
	RID cubeTextureID = loadImage("..\\common\\grid.png");
	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	RID gridShader = ds::createShader();
	ds::loadVertexShader(gridShader, "..\\common\\Textured_vs.cso");
	ds::loadPixelShader(gridShader, "..\\common\\Textured_ps.cso");
	Grid grid;
	v3 gridPositions[] = {
		v3(-4.0f, -1.0f, -3.5f),
		v3(-4.0f, -1.0f,  4.5f),
		v3(4.0f, -1.0f,  4.5f),
		v3(4.0f, -1.0f, -3.5f)
	};
	grid.create(gridPositions, 2, gridShader, textureID);

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	RID rid = ds::createVertexDeclaration(decl, 2, gridShader);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID indexBuffer = ds::createQuadIndexBuffer(256);
	RID cubeBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 24, rid, v, sizeof(Vertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::WRAP, ds::TextureFilters::LINEAR);
	v3 vp = v3(0.0f, 2.0f, -6.0f);
	ds::setViewPosition(vp);
	v3 scale(1.0f, 1.0f, 1.0f);
	v3 rotation(0.0f, 0.0f, 0.0f);
	v3 pos(0.0f, 0.0f, 0.0f);
	FPSCamera camera(1024, 768);
	camera.setPosition(v3(0, 2, -12));
	while (ds::isRunning()) {
		ds::begin();

		camera.update(static_cast<float>(ds::getElapsedSeconds()));
		matrix vpm = camera.getViewProjectionMatrix();
		grid.render(&vpm);

		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;

		ds::setVertexDeclaration(rid);
		ds::setIndexBuffer(indexBuffer);
		ds::setBlendState(bs_id);
		ds::setShader(gridShader);
		ds::setSamplerState(ssid);
		constantBuffer.viewProjectionMatrix = mat_Transpose(camera.getViewProjectionMatrix());

		// spinning cube
		matrix world = mat_identity();
		rotation.y += 2.0f  * static_cast<float>(ds::getElapsedSeconds());
		rotation.x += 1.0f  * static_cast<float>(ds::getElapsedSeconds());
		matrix rotY = mat_RotationY(rotation.y);
		matrix rotX = mat_RotationX(rotation.x);
		matrix rotZ = mat_RotationZ(rotation.z);
		matrix s = mat_Scale(scale);
		matrix w = rotZ * rotY * rotX * s * world;
		constantBuffer.worldMatrix = mat_Transpose(w);
		ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
		ds::setConstantBuffer(cbid, ds::ShaderType::VERTEX);
		ds::setTexture(cubeTextureID, ds::ShaderType::PIXEL);
		ds::setVertexBuffer(cubeBuffer, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
		ds::drawIndexed(36);
		
		ds::end();
	}
	ds::shutdown();

}