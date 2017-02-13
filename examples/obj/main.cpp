#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "WaveFrontReader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct CubeConstantBuffer {
	matrix viewProjectionMatrix;
	matrix worldMatrix;
};

int main(const char** args) {

	ObjVertex vertices[256];
	WaveFrontReader reader;
	int num = reader.load("cube.obj", vertices, 256);
	if ( num == -1) {
		printf("Cannot load file\n");
	}
	
	CubeConstantBuffer constantBuffer;
	float t = 0.0f;
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	if (ds::init(rs)) {

		int x, y, n;
		unsigned char *data = stbi_load("directx-11-logo.png", &x, &y, &n, 4);
		RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
		stbi_image_free(data);


		RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

		RID shaderID = ds::createShader();
		ds::loadVertexShader(shaderID, "Coloured_vs.cso");
		ds::loadPixelShader(shaderID, "Coloured_ps.cso");

		ds::VertexDeclaration decl[] = {
			{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::TEXCOORD,ds::BufferAttributeType::FLOAT,2 },
			{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 }
		};

		RID rid = ds::createVertexDeclaration(decl, 3, shaderID);
		RID cbid = ds::createConstantBuffer(sizeof(CubeConstantBuffer));
		RID indexBufferID = ds::createQuadIndexBuffer(num / 4);
		RID vbid = ds::createVertexBuffer(ds::BufferType::STATIC, num, 0, vertices,sizeof(ObjVertex));
		RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
		v3 vp = v3(0.0f, 0.0f, -6.0f);
		ds::setViewPosition(vp);
		v3 scale(1.0f, 1.0f, 1.0f);
		v3 rotation(0.0f, 0.0f, 0.0f);
		v3 pos(0.0f, 0.0f, 0.0f);

		int q = num / 4 * 6;
		
		while (ds::isRunning()) {
			ds::begin();
			//t += 0.001f;
			//rotation.y += 0.0001f;
			//rotation.x += 0.0001f;
			matrix world = mat_identity();
			matrix rotY = mat_RotationY(rotation.y);
			matrix rotX = mat_RotationX(rotation.x);
			matrix rotZ = mat_RotationZ(rotation.z);
			matrix s = mat_Scale(scale);
			matrix w = rotZ * rotY * rotX * s * world;
			unsigned int stride = sizeof(ObjVertex);
			unsigned int offset = 0;

			ds::setVertexBuffer(vbid, &stride, &offset, ds::PrimitiveTypes::TRIANGLE_STRIP);
			ds::setVertexDeclaration(rid);
			ds::setIndexBuffer(indexBufferID);
			ds::setBlendState(bs_id);
			ds::setShader(shaderID);
			ds::setSamplerState(ssid);
			ds::setTexture(textureID, ds::ShaderType::PIXEL);
			constantBuffer.viewProjectionMatrix = mat_Transpose(ds::getViewProjectionMatrix());
			constantBuffer.worldMatrix = mat_Transpose(w);
			ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(CubeConstantBuffer));
			ds::setVertexConstantBuffer(cbid);
			ds::drawIndexed(q*6);
			//ds::draw(num);
			ds::end();
		}
		ds::shutdown();
	}
}