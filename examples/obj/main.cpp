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

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "Obj_vs.cso" },
		{ ds::ShaderType::PIXEL, "Obj_ps.cso" }
	};

	RID shaderID = ds::createShader(desc, 2);

	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 }
	};

	RID rid = ds::createVertexDeclaration(decl, 2, shaderID);
	RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
	RID indexBufferID = ds::createQuadIndexBuffer(num / 4);
	RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, num, sizeof(ObjVertex), vertices);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
	v3 vp = v3(0.0f, 3.0f, -6.0f);
	ds::setViewPosition(vp);

	v3 scale(1.0f, 1.0f, 1.0f);
	v3 rotation(0.0f, 0.0f, 0.0f);
	v3 pos(0.0f, 0.0f, 0.0f);

	int q = num / 4 * 6;
	float t = 0.0f;

	ds::StateGroup* sg = ds::createStateGroup();
	sg->bindLayout(rid);
	sg->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &constantBuffer);
	sg->bindBlendState(bs_id);
	sg->bindShader(shaderID);
	sg->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	sg->bindVertexBuffer(vbid);
	sg->bindIndexBuffer(indexBufferID);
	sg->bindTexture(textureID, ds::ShaderType::PIXEL, 0);

	ds::DrawCommand drawCmd = { q, ds::DrawType::DT_INDEXED, ds::PrimitiveTypes::TRIANGLE_LIST };

	while (ds::isRunning()) {
		ds::begin();
		
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
		
		constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
		constantBuffer.worldMatrix = mat_Transpose(w);
		ds::submit(drawCmd, sg);

		ds::end();
	}
	ds::shutdown();
}