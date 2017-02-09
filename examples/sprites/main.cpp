#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct Rect {
	float top;
	float left;
	float width;
	float height;

	Rect() : top(0.0f), left(0.0f), width(0.0f), height(0.0f) {}
	Rect(float t, float l, float w, float h) : top(t), left(l), width(w), height(h) {}

};

struct Sprite {

	v2 position;
	v2 scale;
	float rotation;
	Rect texture;
	ds::Color color;

	Sprite() : position(0, 0), scale(1, 1), rotation(0.0f), color(ds::Color(255, 255, 255, 255)) {}

};

struct SpriteVertex {

	v3 position;
	v4 texture;
	v3 size;
	ds::Color color;

	SpriteVertex() : position(0, 0, 0) {}
	SpriteVertex(const v3& p, const v4& t, const ds::Color& c) : position(p), texture(t), color(c) {}
	SpriteVertex(const v2& p, const v4& t, const ds::Color& c) : position(p, 1.0f), texture(t), color(c) {}
	SpriteVertex(const v2& p, const v4& t, const v3& s, const ds::Color& c) : position(p, 1.0f), texture(t), size(s), color(c) {}
};

struct SpriteConstantBuffer {
	v4 screenDimension;
	v4 screenCenter;
	matrix wvp;
};

int add(const v2& p, const Rect& r, Sprite* sprites, int index) {
	if ((index + 1) < 64) {
		Sprite& s = sprites[index];
		s.color = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
		s.scale = v2(1.0f, 1.0f);
		s.rotation = 0.0f;
		s.texture = r;
		return index + 1;
	}
	return index;
}

int main(const char** args) {
	
	Sprite sprites[64];
	SpriteVertex buffer[64];
	int numSprites = 0;

	numSprites = add(v2(512, 364), Rect(0, 0, 256, 256),sprites,numSprites);

	SpriteConstantBuffer constantBuffer;
	constantBuffer.screenDimension = v4(1024.0f, 768.0f, 1024.0f, 1024.0f);
	constantBuffer.screenCenter = v4(512.0f, 384.0f, 0.0f, 0.0f);
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
		ds::loadVertexShader(shaderID, "Sprites_vs.cso");
		ds::loadPixelShader(shaderID, "Sprites_ps.cso");
		ds::loadGeometryShader(shaderID, "Sprites_gs.cso");

		ds::VertexDeclaration decl[] = {
			{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 },
			{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
			{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
		};

		RID vertexDeclId = ds::createVertexDeclaration(decl, 4, shaderID);
		RID cbid = ds::createConstantBuffer(sizeof(SpriteConstantBuffer));
		RID indexBufferID = ds::createQuadIndexBuffer(1024);
		RID vertexBufferID = ds::createVertexBuffer(ds::BufferType::DYNAMIC, 64, vertexDeclId);
		RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);
		matrix viewMatrix = mat_identity();
		matrix projectionMatrix = mat_OrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
		matrix viewProjectionMatrix = viewMatrix * projectionMatrix;
		while (ds::isRunning()) {
			ds::begin();
			unsigned int stride = sizeof(SpriteVertex);
			unsigned int offset = 0;
			ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
			ds::setVertexBuffer(vertexBufferID, &stride, &offset, ds::PrimitiveTypes::POINT_LIST);
			ds::setVertexDeclaration(vertexDeclId);
			ds::setIndexBuffer(indexBufferID);
			ds::setBlendState(bs_id);
			ds::setShader(shaderID);
			ds::setSamplerState(ssid);
			ds::setTexture(textureID);
			for (int i = 0; i < numSprites; ++i) {
				const Sprite& sprite = sprites[i];
				v4 t;
				t.x = sprite.texture.left;
				t.y = sprite.texture.top;
				t.z = sprite.texture.width;
				t.w = sprite.texture.height;
				buffer[i] = SpriteVertex(sprite.position, t, v3(sprite.scale.x, sprite.scale.y, sprite.rotation), sprite.color);
			}
			ds::mapBufferData(vertexBufferID, buffer, numSprites * sizeof(SpriteVertex));
			matrix w = mat_identity();
			constantBuffer.wvp = mat_Transpose(viewProjectionMatrix);
			ds::updateConstantBuffer(cbid,&constantBuffer,sizeof(SpriteConstantBuffer));
			ds::setGeometryConstantBuffer(cbid);
			ds::setVertexConstantBuffer(cbid);
			ds::draw(numSprites);
			ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
			ds::end();
		}
		ds::shutdown();
	}
}