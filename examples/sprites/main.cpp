#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"

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
// The sprite
// ---------------------------------------------------------------
struct Sprite {

	ds::vec2 position;
	ds::vec2 scale;
	float rotation;
	ds::vec4 texture;
	ds::Color color;
	ds::vec2 velocity;

	Sprite() : position(0, 0), scale(1, 1), rotation(0.0f), color(ds::Color(255, 255, 255, 255)) , texture(0,0,0,0) , velocity(0.0f,0.0f) {}

};

// ---------------------------------------------------------------
// The sprite vertex
// ---------------------------------------------------------------
struct SpriteVertex {

	ds::vec3 position;
	ds::vec4 texture;
	ds::vec3 size;
	ds::Color color;

	SpriteVertex() : position(0, 0, 0) {}
	SpriteVertex(const ds::vec3& p, const ds::vec4& t, const ds::Color& c) : position(p), texture(t), color(c) {}
	SpriteVertex(const ds::vec2& p, const ds::vec4& t, const ds::Color& c) : position(p, 1.0f), texture(t), color(c) {}
	SpriteVertex(const ds::vec2& p, const ds::vec4& t, const ds::vec3& s, const ds::Color& c) : position(p, 1.0f), texture(t), size(s), color(c) {}
};

// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct SpriteConstantBuffer {
	ds::vec4 screenDimension;
	ds::vec4 screenCenter;
	ds::matrix wvp;
};

// ---------------------------------------------------------------
// get angle between two ds::vec2 vectors
// ---------------------------------------------------------------
float getAngle(const ds::vec2& u, const ds::vec2& v) {
	double x = v.x - u.x;
	double y = v.y - u.y;
	double ang = atan2(y, x);
	return (float)ang;
}

// ---------------------------------------------------------------
// create sprite
// ---------------------------------------------------------------
int add(const ds::vec2& p, const ds::vec4& r, Sprite* sprites, int index) {
	if ((index + 1) < 64) {
		float angle = ds::random(0.0f, ds::PI * 2.0f);
		Sprite& s = sprites[index];
		s.position = p;
		s.color = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
		s.scale = ds::vec2(1.0f, 1.0f);
		s.rotation = angle;
		s.texture = r;
		float vel = ds::random(100.0f,250.0f);
		float vx = cos(angle) * vel;
		float vy = sin(angle) * vel;
		s.velocity = ds::vec2(vx,vy);
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

	SpriteVertex vertices[64];

	// create some sprites
	for (int i = 0; i < 48; ++i) {
		float x = ds::random(200.0f, 800.0f);
		float y = ds::random(200.0f, 500.0f);
		int r = ds::random(0.0, 1.9f);
		int c = ds::random(0.0, 1.9f);
		numSprites = add(ds::vec2(x, y), ds::vec4(r * 40, c * 40, 40, 40), sprites, numSprites);
	}

	SpriteConstantBuffer constantBuffer;
	constantBuffer.screenDimension = ds::vec4(1024.0f, 768.0f, 128.0f, 128.0f);
	constantBuffer.screenCenter = ds::vec4(512.0f, 384.0f, 0.0f, 0.0f);
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

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "Sprites_vs.cso" },
		{ ds::ShaderType::PIXEL, "Sprites_ps.cso" },
		{ ds::ShaderType::GEOMETRY, "Sprites_gs.cso" }
	};

	RID shaderID = ds::createShader(desc, 3);

	// very special buffer layout 
	ds::VertexDeclaration decl[] = {
		{ ds::BufferAttribute::POSITION,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 },
		{ ds::BufferAttribute::NORMAL,ds::BufferAttributeType::FLOAT,3 },
		{ ds::BufferAttribute::COLOR,ds::BufferAttributeType::FLOAT,4 }
	};

	RID vertexDeclId = ds::createVertexDeclaration(decl, 4, shaderID);
		
	RID cbid = ds::createConstantBuffer(sizeof(SpriteConstantBuffer));
	RID vertexBufferID = ds::createVertexBuffer(ds::BufferType::DYNAMIC, 64, sizeof(SpriteVertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::LINEAR);

	// create orthographic view
	ds::matrix viewMatrix = ds::matIdentity();
	ds::matrix projectionMatrix = ds::matOrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	ds::matrix viewProjectionMatrix = viewMatrix * projectionMatrix;

	ds::setViewMatrix(viewMatrix);
	ds::setProjectionMatrix(projectionMatrix);

	ds::StateGroup* sg = ds::createStateGroup();
	sg->bindLayout(vertexDeclId);
	sg->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, 0, &constantBuffer);
	sg->bindConstantBuffer(cbid, ds::ShaderType::GEOMETRY, 0, &constantBuffer);
	sg->bindBlendState(bs_id);
	sg->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	sg->bindVertexBuffer(vertexBufferID);
	sg->bindShader(shaderID);
	sg->bindTexture(textureID, ds::ShaderType::PIXEL, 0);

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::POINT_LIST, 0 };

	while (ds::isRunning()) {
		// move sprites
		for (int i = 0; i < numSprites; ++i) {
			Sprite& s = sprites[i];
			s.position += s.velocity * static_cast<float>(ds::getElapsedSeconds());
			if (s.position.x < 10.0f || s.position.x > 1000.0f) {
				s.velocity.x *= -1.0f;
				s.position += s.velocity * static_cast<float>(ds::getElapsedSeconds());
				s.rotation = getAngle(s.velocity, ds::vec2(1, 0));
			}
			if (s.position.y < 10.0f || s.position.y > 760.0f) {
				s.velocity.y *= -1.0f;
				s.position += s.velocity * static_cast<float>(ds::getElapsedSeconds());
				s.rotation = getAngle(s.velocity, ds::vec2(1, 0));
			}
		}
			
		ds::begin();
		// disbale depth buffer
		ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
		ds::matrix w = ds::matIdentity();
		constantBuffer.wvp = ds::matTranspose(viewProjectionMatrix);
		for (int i = 0; i < numSprites; ++i) {
			const Sprite& sprite = sprites[i];
			vertices[i] = SpriteVertex(sprite.position, sprite.texture, ds::vec3(sprite.scale.x, sprite.scale.y, sprite.rotation), sprite.color);
		}
		ds::mapBufferData(vertexBufferID, vertices, numSprites * sizeof(SpriteVertex));
		drawCmd.size = numSprites;
		ds::submit(drawCmd, sg);
		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
		ds::end();
	}
	ds::shutdown();
}