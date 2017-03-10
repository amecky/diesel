#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\SpriteBuffer.h"
#include "Board.h"
#include "GameSettings.h"
#include "utils\utils.h"
#include "utils\HUD.h"

enum GameMode {
	GM_MENU,
	GM_RUNNING,
	GM_GAMEOVER
};
// ---------------------------------------------------------------
// initialize rendering system
// ---------------------------------------------------------------
void initialize() {
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Colors - match 3 game";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	initialize();

	SpriteBuffer spriteBuffer(512);
	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("TextureArray.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);

	// prepare the game settings
	GameSettings settings;
	settings.moveInTTL = 0.9f;
	settings.moveInYAdd = 600;
	settings.moveInYOffset = 60;
	settings.flashTTL = 0.5f;
	settings.droppingTTL = 1.0f;

	Board* board = new Board(&spriteBuffer,textureID, &settings);
	board->fill(4);

	HUD hud(&spriteBuffer, textureID);
	hud.reset();

	char txt[256];

	bool pressed = false;

	int score = 0;

	GameMode mode = GM_MENU;

	while (ds::isRunning()) {

		if (ds::isKeyPressed('A')) {
			board->fill(4);
		}
		if (ds::isKeyPressed('C')) {
			board->clearBoard();
		}

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

		if (board->isReady()) {
			hud.tick(static_cast<float>(ds::getElapsedSeconds()));
		}

		ds::begin();

		spriteBuffer.begin();

		spriteBuffer.add(ds::vec2(512, 714), textureID, ds::vec4(0, 720, 1024, 68));
		spriteBuffer.add(ds::vec2(512, 16), textureID, ds::vec4(0, 800, 1024, 68));

		board->render();

		if (mode == GM_RUNNING) {
			hud.render();
		}

		spriteBuffer.flush();
		
		ds::end();
	}
	delete board;
	ds::shutdown();
}