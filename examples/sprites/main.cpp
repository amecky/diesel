#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
	Simple sprite demo.

	The actual sprites are drawn as points and the geometry shader
	will build the quads. Therefore a special layout is used:

	POSITION : the sprite position
	COLOR : the for u/v coordinates
	NORMAL : the scale.x and scale.y along with rotation
	COLOR : the actual color

	The constant buffer contains:
	screenDimension : this is split into the screen dimension and the texture width and height
	screenCenter : the actual center of the screen and the last two components are for padding

	The demo also uses the Batch class which can be used to batch draw calls.
*/
// ---------------------------------------------------------------
// Rect
// ---------------------------------------------------------------
struct Rect {
	float top;
	float left;
	float width;
	float height;

	Rect() : top(0.0f), left(0.0f), width(0.0f), height(0.0f) {}
	Rect(float t, float l, float w, float h) : top(t), left(l), width(w), height(h) {}

};

// ---------------------------------------------------------------
// The sprite
// ---------------------------------------------------------------
struct Sprite {

	v2 position;
	v2 scale;
	float rotation;
	Rect texture;
	ds::Color color;
	v2 velocity;

	Sprite() : position(0, 0), scale(1, 1), rotation(0.0f), color(ds::Color(255, 255, 255, 255)) , texture(0,0,0,0) , velocity(0.0f,0.0f) {}

};

// ---------------------------------------------------------------
// The sprite vertex
// ---------------------------------------------------------------
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

// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct SpriteConstantBuffer {
	v4 screenDimension;
	v4 screenCenter;
	matrix wvp;
};

// ---------------------------------------------------------------
// get angle between two v2 vectors
// ---------------------------------------------------------------
float getAngle(const v2& u, const v2& v) {
	double x = v.x - u.x;
	double y = v.y - u.y;
	double ang = atan2(y, x);
	return (float)ang;
}

// ---------------------------------------------------------------
// create sprite
// ---------------------------------------------------------------
int add(const v2& p, const Rect& r, Sprite* sprites, int index) {
	if ((index + 1) < 64) {
		float angle = ds::random(0.0f, ds::PI * 2.0f);
		Sprite& s = sprites[index];
		s.position = p;
		s.color = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
		s.scale = v2(1.0f, 1.0f);
		s.rotation = angle;
		s.texture = r;
		float vel = ds::random(100.0f,150.0f);
		float vx = cos(angle) * vel;
		float vy = sin(angle) * vel;
		s.velocity = v2(vx,vy);
		return index + 1;
	}
	return index;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	Sprite sprites[64];
	int numSprites = 0;

	// create some sprites
	for (int i = 0; i < 48; ++i) {
		float x = ds::random(200.0f, 800.0f);
		float y = ds::random(200.0f, 500.0f);
		int r = ds::random(0.0, 1.9f);
		int c = ds::random(0.0, 1.9f);
		numSprites = add(v2(x, y), Rect(r * 40, c * 40, 40, 40), sprites, numSprites);
	}

	SpriteConstantBuffer constantBuffer;
	constantBuffer.screenDimension = v4(1024.0f, 768.0f, 128.0f, 128.0f);
	constantBuffer.screenCenter = v4(512.0f, 384.0f, 0.0f, 0.0f);
	float t = 0.0f;

	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("cubes.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);


	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	// create shader and load compiled shaders
	RID shaderID = ds::createShader();
	ds::loadVertexShader(shaderID, "Sprites_vs.cso");
	ds::loadPixelShader(shaderID, "Sprites_ps.cso");
	ds::loadGeometryShader(shaderID, "Sprites_gs.cso");

	// very special buffer layout 
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	RID vertexDeclId = ds::createVertexDeclaration(decl, 4, shaderID);
		
	RID cbid = ds::createConstantBuffer(sizeof(SpriteConstantBuffer));
	RID vertexBufferID = ds::createVertexBuffer(ds::BufferType::DYNAMIC, 64, vertexDeclId);
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	// create orthographic view
	matrix viewMatrix = mat_identity();
	matrix projectionMatrix = mat_OrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	matrix viewProjectionMatrix = viewMatrix * projectionMatrix;

	// batch descriptor used to initialize batch
	ds::BatchDescriptor desc;
	desc.size = 32;
	desc.vertexBufferID = vertexBufferID;
	desc.primitiveType = ds::PrimitiveTypes::POINT_LIST;
	desc.vertexDeclarationID = vertexDeclId;
	// use batch
	ds::Batch<SpriteVertex> batch(desc);

	while (ds::isRunning()) {
		// move sprites
		for (int i = 0; i < numSprites; ++i) {
			Sprite& s = sprites[i];
			s.position += s.velocity * static_cast<float>(ds::getElapsedSeconds());
			if (s.position.x < 10.0f || s.position.x > 1000.0f) {
				s.velocity.x *= -1.0f;
				s.position += s.velocity * static_cast<float>(ds::getElapsedSeconds());
				s.rotation = getAngle(s.velocity, v2(1, 0));
			}
			if (s.position.y < 10.0f || s.position.y > 760.0f) {
				s.velocity.y *= -1.0f;
				s.position += s.velocity * static_cast<float>(ds::getElapsedSeconds());
				s.rotation = getAngle(s.velocity, v2(1, 0));
			}
		}
			
		ds::begin();
		// disbale depth buffer
		ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
		ds::setBlendState(bs_id);
		ds::setShader(shaderID);
		ds::setSamplerState(ssid);
		ds::setTexture(textureID, ds::ShaderType::PIXEL);
		// prepare constant buffers
		matrix w = mat_identity();
		constantBuffer.wvp = mat_Transpose(viewProjectionMatrix);
		ds::updateConstantBuffer(cbid, &constantBuffer, sizeof(SpriteConstantBuffer));
		ds::setGeometryConstantBuffer(cbid);
		ds::setVertexConstantBuffer(cbid);
		// use batch to draw sprites
		batch.begin();
		for (int i = 0; i < numSprites; ++i) {
			const Sprite& sprite = sprites[i];
			v4 t;
			t.x = sprite.texture.left;
			t.y = sprite.texture.top;
			t.z = sprite.texture.width;
			t.w = sprite.texture.height;
			batch.add(SpriteVertex(sprite.position, t, v3(sprite.scale.x, sprite.scale.y, sprite.rotation), sprite.color));
		}
		batch.flush();
		// enable depth buffer
		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
		ds::end();
	}
	ds::shutdown();
}