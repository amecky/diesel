#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "WaveFrontReader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct CubeConstantBuffer {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
};

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	ObjVertex vertices[256];
	WaveFrontReader reader;
	int num = reader.load("cube.obj", vertices, 256);
	if ( num == -1) {
		printf("Cannot load file\n");
	}
	
	CubeConstantBuffer constantBuffer;
	
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	int x, y, n;
	unsigned char *data = stbi_load("..\\common\\cube_map.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);


	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	RID shaderID = ds::createShader();
	ds::loadVertexShader(shaderID, "Obj_vs.cso");
	ds::loadPixelShader(shaderID, "Obj_ps.cso");

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	ObjVertex floorVertices[4];
	floorVertices[0] = ObjVertex(v3(-3.0f, -1.0f, -2.5f), v2(0.0f, 1.0f));
	floorVertices[1] = ObjVertex(v3(-3.0f, -1.0f, 3.5f), v2(0.0f, 0.0f));
	floorVertices[2] = ObjVertex(v3(3.0f, -1.0f, 3.5f), v2(1.0f, 0.0f));
	floorVertices[3] = ObjVertex(v3(3.0f, -1.0f, -2.5f), v2(1.0f, 1.0f));

	RID rid = ds::createVertexDeclaration(decl, 2, shaderID);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID indexBufferID = ds::createQuadIndexBuffer(num / 4);
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, num, rid, vertices,sizeof(ObjVertex));
	RID floorBuffer = ds::createVertexBuffer(ds::BufferType::STATIC, 4, rid, floorVertices, sizeof(ObjVertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	v3 vp = v3(0.0f, 3.0f, -6.0f);
	ds::setViewPosition(vp);

	v3 scale(1.0f, 1.0f, 1.0f);
	v3 rotation(0.0f, 0.0f, 0.0f);
	v3 pos(0.0f, 0.0f, 0.0f);

	int q = num / 4 * 6;
	float t = 0.0f;
	while (ds::isRunning()) {
		ds::begin();
		unsigned int stride = sizeof(ObjVertex);
		unsigned int offset = 0;

		// set floor buffer
		ds::setVertexBuffer(floorBuffer, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
		// this is common to both objects
		ds::setVertexDeclaration(rid);
		ds::setIndexBuffer(indexBufferID);
		ds::setBlendState(bs_id);
		ds::setShader(shaderID);
		ds::setSamplerState(ssid);
		ds::setTexture(textureID, ds::ShaderType::PIXEL);
		// update constant buffer use v3(0,0,0) as world position
		constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
		constantBuffer.worldMatrix = mat_Transpose(mat_identity());
		ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
		ds::setVertexConstantBuffer(cbid);
		// draw floor
		ds::drawIndexed(6);


		// move cube
		rotation.y += 2.0f  * static_cast<float>(ds::getElapsedSeconds());
		rotation.x += 1.0f  * static_cast<float>(ds::getElapsedSeconds());
		t += static_cast<float>(ds::getElapsedSeconds());
		v3 scale = v3(0.9f + sin(t * 4.0f) * 0.2f);
		matrix world = mat_identity();
		matrix rotY = mat_RotationY(rotation.y);
		matrix rotX = mat_RotationX(rotation.x);
		matrix rotZ = mat_RotationZ(rotation.z);
		matrix s = mat_Scale(scale);
		matrix w = rotZ * rotY * rotX * s * world;
		
		// switch vertex buffer
		ds::setVertexBuffer(vbid, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_LIST);
		// update constant buffer
		constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
		constantBuffer.worldMatrix = mat_Transpose(w);
		ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
		ds::setVertexConstantBuffer(cbid);
		// draw cube
		ds::drawIndexed(q);
		ds::end();
	}
	ds::shutdown();
}