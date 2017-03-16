#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#include "HieroFont.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"

/*
	Bitmap font demo. The font is generated using Hiero: https://github.com/libgdx/libgdx/wiki/Hiero
*/

const float TTL[] = { 0.5f,2.0f,0.5f };

enum MessageState {
	MS_FADE_IN,
	MS_SHOW,
	MS_FADE_OUT
};

struct Message {
	const char* text;
	MessageState state;
	float timer;
};
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

const int MAX_SPRITES = 256;
Sprite sprites[MAX_SPRITES];
int numSprites;

// ---------------------------------------------------------------
// create sprite
// ---------------------------------------------------------------
void add(const ds::vec2& p, const Rect& r) {
	if ((numSprites + 1) < MAX_SPRITES) {
		float angle = 0.0f;
		Sprite& s = sprites[numSprites++];
		s.position = p;
		s.color = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
		s.scale = ds::vec2(1.0f, 1.0f);
		s.rotation = angle;
		s.texture = r;
		float vel = ds::random(100.0f,150.0f);
		float vx = cos(angle) * vel;
		float vy = sin(angle) * vel;
		s.velocity = ds::vec2(vx,vy);
	}
}

// ---------------------------------------------------------------
// add text
// ---------------------------------------------------------------
void addText(const HieroFont& font, const ds::vec2& pos, const char* text) {
	float xpos = pos.x;
	float ypos = pos.y;
	const char* current = text;
	for (int i = 0; i < strlen(text); ++i) {
		const HieroFontItem& item = font.get(*current);
		float dimX = item.width;
		float dimY = item.height;
		float th = item.height + item.yoffset;
		add(ds::vec2(xpos + dimX * 0.5f, ypos - th - item.yoffset * 0.5f), Rect(item.y, item.x, item.width, item.height));
		xpos += item.xadvance + 4;
		++current;
	}
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	HieroFont font;
	font.load("verdana_32.fnt");

	
	std::vector<Message> messages;
	Message msg1;
	msg1.state = MS_FADE_IN;
	msg1.text = "Bitmap Font Demo";
	msg1.timer = 0.0f;
	messages.push_back(msg1);
	Message msg2;
	msg2.state = MS_FADE_IN;
	msg2.text = "Simple text fading demo";
	msg2.timer = 0.0f;
	messages.push_back(msg2);
	size_t currentMessage = 0;

	SpriteVertex vertices[256];
	int numVertices = 0;

	//addText(font, ds::vec2(300.0f, 500.0f), "More text is here");
	//addText(font, ds::vec2(600.0f, 100.0f), "Here is also some text");

	SpriteConstantBuffer constantBuffer;
	constantBuffer.screenDimension = ds::vec4(1024.0f, 768.0f, 256.0f, 256.0f);
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
	unsigned char *data = stbi_load("verdana_32.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);


	RID bs_id = ds::createBlendState(ds::BlendStates::SRC_ALPHA, ds::BlendStates::SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, ds::BlendStates::INV_SRC_ALPHA, true);

	ds::ShaderDescriptor desc[] = {
		{ ds::ShaderType::VERTEX, "Font_vs.cso" },
		{ ds::ShaderType::PIXEL, "Font_ps.cso" },
		{ ds::ShaderType::GEOMETRY, "Font_gs.cso" }
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
	RID vertexBufferID = ds::createVertexBuffer(ds::BufferType::DYNAMIC, MAX_SPRITES, sizeof(SpriteVertex));
	RID ssid = ds::createSamplerState(ds::TextureAddressModes::CLAMP, ds::TextureFilters::POINT);

	// create orthographic view
	ds::matrix viewMatrix = ds::matIdentity();
	ds::matrix projectionMatrix = ds::matOrthoLH(1024.0f, 768.0f, 0.1f, 1.0f);
	ds::matrix viewprojectionMatrix = viewMatrix * projectionMatrix;

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

	ds::DrawCommand drawCmd = { 100, ds::DrawType::DT_VERTICES, ds::PrimitiveTypes::POINT_LIST };

	while (ds::isRunning()) {
		ds::begin();
		//ds::Color textColor(255, 255, 255, 255);
		Message& m = messages[currentMessage];
		m.timer += static_cast<float>(ds::getElapsedSeconds());
		if (m.timer >= TTL[m.state]) {
			m.timer = 0;
			switch (m.state) {
			case MS_FADE_IN: m.state = MS_SHOW; break;
			case MS_SHOW: m.state = MS_FADE_OUT; break;
			case MS_FADE_OUT:
				++currentMessage;
				if (currentMessage >= messages.size()) {
					currentMessage = 0;
				}
				messages[currentMessage].state = MS_FADE_IN;
				messages[currentMessage].timer = 0.0f;
				numSprites = 0;
				// FIXME: center text
				addText(font, ds::vec2(100.0f,384.0f),messages[currentMessage].text);
			}
		}
		ds::Color textColor = ds::Color(192, 0, 0, 255);
		if (m.state == MS_FADE_IN) {
			textColor.a = m.timer / TTL[0];
		}
		else if (m.state == MS_FADE_OUT) {
			textColor.a = 1.0f - m.timer / TTL[0];
		}
		else {
			textColor.a = 1.0f;
		}
		// disbale depth buffer
		ds::setDepthBufferState(ds::DepthBufferState::DISABLED);
		ds::matrix w = ds::matIdentity();
		constantBuffer.wvp = ds::matTranspose(viewprojectionMatrix);
		numVertices = 0;
		for (int i = 0; i < numSprites; ++i) {
			const Sprite& sprite = sprites[i];
			ds::vec4 t;
			t.x = sprite.texture.left;
			t.y = sprite.texture.top;
			t.z = sprite.texture.width;
			t.w = sprite.texture.height;
			vertices[numVertices++] = SpriteVertex(sprite.position, t, ds::vec3(sprite.scale.x, sprite.scale.y, sprite.rotation), textColor);
		}
		ds::mapBufferData(vertexBufferID, vertices, numVertices * sizeof(SpriteVertex));
		drawCmd.size = numSprites;
		ds::submit(drawCmd, sg);
		// enable depth buffer
		ds::setDepthBufferState(ds::DepthBufferState::ENABLED);
		ds::end();
	}
	ds::shutdown();
}