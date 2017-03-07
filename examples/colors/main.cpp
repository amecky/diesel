#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "..\common\SpriteBuffer.h"
#include "Board.h"
#include "GameSettings.h"
#include "utils\utils.h"
#include "utils\HUD.h"

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
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	// prepare application
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);


	SpriteBuffer spriteBuffer(512);
	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("TextureArray.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);

	// prepare the game settings
	GameSettings settings;
	settings.tweeningTTL = 2.0f;
	settings.tweeningYAdd = 100;
	settings.tweeningYOffset = 300;
	settings.flashTTL = 0.5f;
	settings.droppingTTL = 1.0f;

	Board* board = new Board(&spriteBuffer,textureID, &settings);
	board->fill(4);

	HUD hud(&spriteBuffer, textureID);
	hud.reset();

	char txt[256];

	bool pressed = false;

	int score = 0;

	while (ds::isRunning()) {

		if (ds::isMouseButtonPressed(0) && !pressed) {
			int points = board->select();
			if (points > 0) {
				score += points * 10;
				hud.setNumber(score);
			}
			pressed = true;
		}
		if (!ds::isMouseButtonPressed(0) && pressed) {
			pressed = false;
		}

		board->update(static_cast<float>(ds::getElapsedSeconds()));

		hud.tick(static_cast<float>(ds::getElapsedSeconds()));

		ds::begin();

		spriteBuffer.begin();

		spriteBuffer.add(v2(512, 734), textureID, v4(0, 720, 1024, 68));
		spriteBuffer.add(v2(512, 16), textureID, v4(0, 800, 1024, 68));

		board->render();

		hud.render();

		spriteBuffer.flush();
		
		ds::end();
	}
	delete board;
	ds::shutdown();
}