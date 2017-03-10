#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

	ds::vec2 position;
	ds::vec2 scale;
	float rotation;
	Rect texture;
	ds::Color color;
	ds::vec2 velocity;
	float timer;

	Sprite() : position(0, 0), scale(1, 1), rotation(0.0f), color(ds::Color(255, 255, 255, 255)) , texture(0,0,0,0) , velocity(0.0f,0.0f) , timer(0.0f) {}

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
int add(const ds::vec2& p, const Rect& r, Sprite* sprites, int index) {
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
		s.timer = ds::random(0.0f, 1.0f);
		return index + 1;
	}
	return index;
}

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
	
	Sprite sprites[64];
	int numSprites = 0;

	SpriteVertex vertices[64];

	SpriteVertex backgroundVertex;
	backgroundVertex.position = ds::vec3(512, 384, 0);
	backgroundVertex.texture = ds::vec4(0, 200, 1024, 768);
	backgroundVertex.size = ds::vec3(1.0f,1.0f,0.0f);
	backgroundVertex.color = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
	
	// create some sprites
	for (int i = 0; i < 4; ++i) {
		float x = ds::random(200.0f, 800.0f);
		float y = ds::random(200.0f, 500.0f);
		int r = ds::random(0.0, 1.9f);
		int c = ds::random(0.0, 1.9f);
		numSprites = add(ds::vec2(x, y), Rect(0,0,64,64), sprites, numSprites);
	}

	SpriteConstantBuffer constantBuffer;
	constantBuffer.screenDimension = ds::vec4(1024.0f, 768.0f, 64.0f, 64.0f);
	constantBuffer.screenCenter = ds::vec4(512.0f, 384.0f, 0.0f, 0.0f);

	float t = 0.0f;

	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Ripples demo";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	RID textureID = loadImage("Blast.png");	
	// Image is taken from here: http://smnbrnr.deviantart.com/art/Martian-Oasis-479173854
	RID bgTextureID = loadImage("martian_oasis_by_smnbrnr.png");


	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "Sprites_vs.cso" },
		{ ds::ShaderType::PIXEL, "Sprites_ps.cso" },
		{ ds::ShaderType::GEOMETRY, "Sprites_gs.cso" }
	};

	RID shaderID = ds::createShader(desc, 3);

	ds::ShaderDescriptor rpDesc[] = {
		{ ds::ShaderType::VERTEX, "Ripple_vs.cso" },
		{ ds::ShaderType::PIXEL, "Ripple_ps.cso" }
	};

	RID rippleShaderID = ds::createShader(rpDesc, 2);

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
	ds::matrix viewprojectionMatrix = viewMatrix * projectionMatrix;

	ds::setviewMatrix(viewMatrix);
	ds::setprojectionMatrix(projectionMatrix);
	constantBuffer.wvp = ds::matTranspose(viewprojectionMatrix);

	ds::StateGroup* sg = ds::createStateGroup();
	sg->bindLayout(vertexDeclId);
	sg->bindConstantBuffer(cbid, ds::ShaderType::VERTEX, &constantBuffer);
	sg->bindConstantBuffer(cbid, ds::ShaderType::GEOMETRY, &constantBuffer);
	sg->bindBlendState(bs_id);
	sg->bindSamplerState(ssid, ds::ShaderType::PIXEL);
	sg->bindVertexBuffer(vertexBufferID);
	sg->bindShader(shaderID);
	sg->bindTexture(textureID, ds::ShaderType::PIXEL, 0);


	RID rt = ds::createRenderTarget(1024, 768, ds::Color(0.0f,0.0f,0.0f,1.0f));

	ds::StateGroup* bgStateGroup = ds::createStateGroup();
	bgStateGroup->bindLayout(NO_RID);
	bgStateGroup->bindIndexBuffer(NO_RID);
	bgStateGroup->bindVertexBuffer(NO_RID);
	bgStateGroup->bindBlendState(bs_id);
	bgStateGroup->bindShader(rippleShaderID);
	bgStateGroup->bindTextureFromRenderTarget(rt, ds::ShaderType::PIXEL, 0);
	bgStateGroup->bindTexture(bgTextureID, ds::ShaderType::PIXEL, 1);

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::POINT_LIST, 0 };

	ds::DrawCommand rtDrawCmd = { 3, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::TRIANGLE_LIST, 0 };

	

	while (ds::isRunning()) {
		// move sprites		
		for (int i = 0; i < numSprites; ++i) {
			Sprite& s = sprites[i];
			s.timer += static_cast<float>(ds::getElapsedSeconds());
			float norm = s.timer / 2.0f;
			s.scale = ds::vec2(sin(norm * ds::TWO_PI) * 4.0f);
			if (norm >= 1.0f) {
				s.timer = 0.1f;
				float x = ds::random(100.0f, 900.0f);
				float y = ds::random(100.0f, 600.0f);
				s.position = ds::vec2(x, y);
			}
			s.rotation = 0.0f;

		}
		
		ds::begin();
		// disbale depth buffer
		ds::setDepthBufferState(ds::DepthBufferState::DISABLED);

		ds::setRenderTarget(rt);
		
		for (int i = 0; i < numSprites; ++i) {
			const Sprite& sprite = sprites[i];
			ds::vec4 t;
			t.x = sprite.texture.left;
			t.y = sprite.texture.top;
			t.z = sprite.texture.width;
			t.w = sprite.texture.height;
			vertices[i] = SpriteVertex(sprite.position, t, ds::vec3(sprite.scale.x, sprite.scale.y, sprite.rotation), sprite.color);
		}
		ds::mapBufferData(vertexBufferID, vertices, numSprites * sizeof(SpriteVertex));
		drawCmd.size = numSprites;
		ds::submit(drawCmd, sg);

		ds::restoreBackBuffer();
		
		

		ds::submit(rtDrawCmd, bgStateGroup);
		
		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);

		ds::end();
	}
	ds::shutdown();
}