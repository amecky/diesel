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

	ds::ShaderDescriptor gridDesc[] = {
		{ ds::ShaderType::VERTEX, "..\\common\\Textured_vs.cso" },
		{ ds::ShaderType::PIXEL, "..\\common\\Textured_ps.cso" }
	};

	RID gridShader = ds::createShader(gridDesc, 2);

	Grid grid;
	v3 gridPositions[] = {
		v3(-4.0f, -1.0f, -3.5f),
		v3(-4.0f, -1.0f,  4.5f),
		v3(4.0f, -1.0f,  4.5f),
		v3(4.0f, -1.0f, -3.5f)
	};
	grid.create(gridPositions, 4, gridShader, textureID);

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

	ds::StateGroup* sg = ds::createStateGroup();
	sg->bindLayout(rid);
	sg->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &constantBuffer);
	sg->bindBlendState(bs_id);
	sg->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	sg->bindVertexBuffer(cubeBuffer);
	sg->bindShader(gridShader);
	sg->bindIndexBuffer(indexBuffer);
	ds::DrawCommand drawCmd = { 36, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	while (ds::isRunning()) {
		ds::begin();

		camera.update(static_cast<float>(ds::getElapsedSeconds()));
		grid.render();

		unsigned int stride = sizeof(Vertex);
		unsigned int offset = 0;
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
		//ds::setTexture(cubeTextureID, ds::ShaderType::PIXEL);
		ds::submit(drawCmd, sg);

		ds::end();
	}
	ds::shutdown();

}