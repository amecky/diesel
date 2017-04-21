#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\SpriteBuffer.h"
#include "Board.h"
#include "GameSettings.h"
#include "utils\utils.h"
#include "utils\HUD.h"
#include "Dialog.h"

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
// show game over menu
// ---------------------------------------------------------------
int showGameOverMenu(const Score& score) {
	int ret = 0;
	char buffer[256];
	gui::begin();
	gui::Image(ds::vec2(512, 620), ds::vec4(0, 880, 640, 56));
	ds::vec2 mp = ds::getMousePosition();
	sprintf_s(buffer, 256, "Pieces cleared: %d", score.itemsCleared);
	gui::Text(ds::vec2(400, 500), buffer);
	sprintf_s(buffer, 256, "Time: %02d:%02d", score.minutes, score.seconds);
	gui::Text(ds::vec2(400, 450), buffer);
	sprintf_s(buffer, 256, "Score: %d", score.points);
	gui::Text(ds::vec2(400, 400), buffer);
	//gui::Text(ds::vec2(400, 350), "Total score: 10456");
	if (gui::Button(ds::vec2(512, 320), ds::vec4(0, 70, 260, 60))) {
		ret = 1;
	}
	if (gui::Button(ds::vec2(512, 230), ds::vec4(270, 130, 260, 60))) {
		ret = 2;
	}
	gui::end();
	return ret;
}

// ---------------------------------------------------------------
// show main menu
// ---------------------------------------------------------------
int showMainMenu() {
	int ret = 0;
	gui::begin();
	if (gui::Button(ds::vec2(512, 438), ds::vec4(0, 70, 260, 60))) {
		ret = 1;
	}
	if (gui::Button(ds::vec2(512, 298), ds::vec4(270, 130, 260, 60))) {
		ret = 2;
	}
	gui::end();
	return ret;
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	
	initialize();

	
	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("TextureArray.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);

	SpriteBuffer spriteBuffer(512, textureID);

	// prepare the game settings
	GameSettings settings;
	settings.moveInTTL = 0.9f;
	settings.moveInYAdd = 600;
	settings.moveInYOffset = 60;
	settings.flashTTL = 0.5f;
	settings.droppingTTL = 0.4f;

	Board* board = new Board(&spriteBuffer,textureID, &settings);

	Score score;

	HUD hud(&spriteBuffer, textureID, &score);
	hud.reset();

	char txt[256];

	bool pressed = false;

	GameMode mode = GM_MENU;

	gui::init(&spriteBuffer, textureID);
	bool running = true;

	while (ds::isRunning() && running) {

		if (ds::isKeyPressed('C')) {
			board->clearBoard();
			mode = GM_GAMEOVER;
		}

		ds::begin();

		spriteBuffer.begin();

		spriteBuffer.add(ds::vec2(512, 734), ds::vec4(0, 720, 1024, 68));
		spriteBuffer.add(ds::vec2(512, 34), ds::vec4(0, 800, 1024, 68));

		if (mode == GM_RUNNING) {
			board->render();
		}

		if (mode == GM_MENU) {
			int ret = showMainMenu();
			if (ret == 1) {
				board->fill(4);
				mode = GM_RUNNING;
			}
			else  if (ret == 2) {
				running = false;
			}
		}
		else if (mode == GM_GAMEOVER) {
			int ret = showGameOverMenu(score);
			if (ret == 1) {
				board->fill(4);
				mode = GM_RUNNING;
			}
			else if (ret == 2) {
				mode = GM_MENU;
			}
		}
		else if (mode == GM_RUNNING) {
			if (ds::isMouseButtonPressed(0) && !pressed) {
				int points = board->select();
				if (points > 0) {
					score.points += points * 10;
					score.itemsCleared += points;
					hud.rebuildScore();
				}
				pressed = true;
			}
			if (!ds::isMouseButtonPressed(0) && pressed) {
				pressed = false;
			}

			

			if (board->isReady()) {
				hud.tick(static_cast<float>(ds::getElapsedSeconds()));
			}
		}

		if (mode == GM_RUNNING || mode == GM_GAMEOVER) {
			board->update(static_cast<float>(ds::getElapsedSeconds()));
		}

		if (mode == GM_RUNNING) {
			hud.render();
		}
		
		spriteBuffer.flush();
		
		ds::end();
	}
	delete board;
	ds::shutdown();
}