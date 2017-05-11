#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#define SPRITE_IMPLEMENTATION
#include "..\common\SpriteBatchBuffer.h"

struct TextEntry {
	ds::vec2 pos;
	ds::vec4 rects[64];
	int num;
	ds::Color color;
};

void fillTextEntry(const char* text, TextEntry* entry, int max) {
	const char* current = text;
	int cnt = 0;
	for (int i = 0; i < strlen(text); ++i) {
		int c = current[i] - 32;
		int t = c / 16;
		int y = t * 20;
		int x = (c - t * 16) * 16;
		float dimX = 32;
		float dimY = 32;
		float th = 32;
		if (cnt < max) {
			entry->rects[cnt++] = ds::vec4(x, y, 14.0f, 20.0f);
		}			
	}
	entry->num = cnt;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	numSprites = 0;

	TextEntry entries[3];
	TextEntry& first = entries[0];
	first.pos = ds::vec2(300, 500);
	first.color = ds::Color(192, 0, 0, 255);
	fillTextEntry("This is the first entry", &first, 64);
	TextEntry& second = entries[1];
	second.pos = ds::vec2(100, 100);
	second.color = ds::Color(0, 255, 255, 255);
	fillTextEntry("and yet another text entry", &second, 64);
	TextEntry& third = entries[2];
	third.pos = ds::vec2(50, 300);
	third.color = ds::Color(255, 255, 255, 255);
	fillTextEntry("The final words are here", &third, 64);

	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Simple sprite font demo";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);

	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("nk_57_monospace.png", &x, &y, &n, 4);
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(texInfo);
	stbi_image_free(data);

	SpriteBatchBufferInfo info = { 1024, textureID };
	SpriteBatchBuffer buffer(info);

	while (ds::isRunning()) {

		ds::begin();

		buffer.begin();

		for (int j = 0; j < 3; ++j) {
			const TextEntry& entry = entries[j];
			ds::vec2 p = entry.pos;
			for (int i = 0; i < entry.num; ++i) {
				buffer.add(p, entry.rects[i], ds::vec2(1,1),0.0f,entry.color);
				p.x += 12;
			}
		}
		buffer.flush();

		ds::end();
	}
	ds::shutdown();
}