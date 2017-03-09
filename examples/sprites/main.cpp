#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "..\common\math.h"
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

*/
// ---------------------------------------------------------------
// The sprite
// ---------------------------------------------------------------
struct Sprite {

	v2 position;
	v2 scale;
	float rotation;
	v4 texture;
	v4 color;
	v2 velocity;

	Sprite() : position(0, 0), scale(1, 1), rotation(0.0f), color(1.0f,1.0f,1.0f,1.0f) , texture(0,0,0,0) , velocity(0.0f,0.0f) {}

};

// ---------------------------------------------------------------
// The sprite vertex
// ---------------------------------------------------------------
struct SpriteVertex {

	float position[3];
	float texture[4];
	float size[3];
	float color[4];

};

// ---------------------------------------------------------------
// the sprite constant buffer
// ---------------------------------------------------------------
struct SpriteConstantBuffer {
	float screenDimension[4];
	float screenCenter[4];
	float wvp[16];
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
int add(const v2& p, const v4& r, Sprite* sprites, int index) {
	if ((index + 1) < 64) {
		float angle = ds::random(0.0f, ds::PI * 2.0f);
		Sprite& s = sprites[index];
		s.position = p;
		s.color = v4(1.0f, 1.0f, 1.0f, 1.0f);
		s.scale = v2(1.0f, 1.0f);
		s.rotation = angle;
		s.texture = r;
		float vel = ds::random(100.0f,250.0f);
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

	SpriteVertex vertices[64];

	// create some sprites
	for (int i = 0; i < 48; ++i) {
		float x = ds::random(200.0f, 800.0f);
		float y = ds::random(200.0f, 500.0f);
		int r = ds::random(0.0, 1.9f);
		int c = ds::random(0.0, 1.9f);
		numSprites = add(v2(x, y), v4(r * 40, c * 40, 40, 40), sprites, numSprites);
	}

	SpriteConstantBuffer constantBuffer;
	ds::vec4(constantBuffer.screenDimension, 1024.0f, 768.0f, 128.0f, 128.0f);
	ds::vec4(constantBuffer.screenCenter, 512.0f, 384.0f, 0.0f, 0.0f);
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
	float viewMatrix[16];
	ds::matIdentity(viewMatrix);
	float projectionMatrix[16];
	ds::matOrthoLH(projectionMatrix, 1024.0f, 768.0f, 0.1f, 1.0f);
	float viewProjectionMatrix[16];
	ds::matMultiply(viewProjectionMatrix, viewMatrix, projectionMatrix);

	ds::setViewMatrix(viewMatrix);
	ds::setProjectionMatrix(projectionMatrix);

	ds::StateGroup* sg = ds::createStateGroup();
	sg->bindLayout(vertexDeclId);
	sg->bindConstantBuffer(cbid, ds::ShaderType::GEOMETRY, &constantBuffer);
	sg->bindBlendState(bs_id);
	sg->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	sg->bindVertexBuffer(vertexBufferID);
	sg->bindShader(shaderID);
	sg->bindTexture(textureID, ds::ShaderType::PIXEL, 0);

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::POINT_LIST, 0 };

	float world[16];
	ds::matIdentity(world);
	ds::matTranspose(constantBuffer.wvp, viewProjectionMatrix);

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
		
		for (int i = 0; i < numSprites; ++i) {
			const Sprite& sprite = sprites[i];
			ds::vec3(vertices[i].position, sprite.position.x,sprite.position.y,0.0f);
			ds::vec4(vertices[i].texture, sprite.texture());
			ds::vec3(vertices[i].size, sprite.scale.x, sprite.scale.y, sprite.rotation);
			ds::vec4(vertices[i].color, sprite.color());
		}
		ds::mapBufferData(vertexBufferID, vertices, numSprites * sizeof(SpriteVertex));
		drawCmd.size = numSprites;
		ds::submit(drawCmd, sg);
		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
		ds::end();
	}
	ds::shutdown();
}