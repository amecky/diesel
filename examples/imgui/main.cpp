#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\SpriteBuffer.h"
#include "Dialog.h"

enum GameMode {
	GM_MENU,
	GM_RUNNING,
	GM_GAMEOVER
};

struct TestSettings {
	int iv;
	float fv;
	bool bv;
	int stepValue;
	ds::vec2 v2;
	ds::vec3 v3;
	int state;
	ds::Color color;
	float sinTable[36];
	float hTable[16];
};
// ---------------------------------------------------------------
// initialize rendering system
// ---------------------------------------------------------------
void initialize() {
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "IMGUI Demo";
	rs.clearColor = ds::Color(0.0f, 0.0f, 0.0f, 1.0f);
	rs.multisampling = 4;
	ds::init(rs);
}

char* message = "Hello World";
// ---------------------------------------------------------------
// show game over menu
// ---------------------------------------------------------------
int showDialog(TestSettings* settings) {
	int ret = 0;
	gui::start(ds::vec2(0, 750));
	gui::begin("Basic elements",&settings->state);
	if (settings->state == 1) {
		ds::vec2 mp = ds::getMousePosition();
		gui::Value("Mouse Position", mp);
		gui::Text("Simple text example");
		gui::Label("Pos", "100.0 200.0");
		gui::Label("Message", "Hello");
		gui::Input("Value", &settings->iv);
		gui::Input("Float Value", &settings->fv);
		gui::Checkbox("Check me", &settings->bv);
		gui::Separator();
		gui::StepInput("Step input", &settings->stepValue, 0, 100, 5);
		gui::Input("Vec2 value", &settings->v2);
		gui::Input("Vec3 value", &settings->v3);
		gui::Input("Color value", &settings->color);
		gui::Histogram(settings->hTable, 16, 0.0f, 20.0f, 5.0f);
		gui::Diagram(settings->sinTable, 36, -1.0f, 1.0f, 0.5f);
		gui::beginGroup();
		if (gui::Button("OK")) {
			ret = 1;
		}
		if (gui::Button("Cancel")) {
			ret = 2;
		}
		gui::endGroup();
	}
	gui::end();
	return ret;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
int main(int argc, char *argv[]) {
	
	initialize();

	SpriteBuffer spriteBuffer(512);
	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("imgui.png", &x, &y, &n, 4);
	RID textureID = ds::createTexture(x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM);
	stbi_image_free(data);

	gui::init(&spriteBuffer, textureID);

	TestSettings settings;
	settings.bv = true;
	settings.stepValue = 20;
	settings.iv = 120;
	settings.fv = 4.0f;
	settings.v2 = ds::vec2(100, 200);
	settings.v3 = ds::vec3(100, 200, 300);
	settings.color = ds::Color(192, 32, 64, 255);
	settings.state = 1;
	for (int i = 0; i < 36; ++i) {
		settings.sinTable[i] = sin(static_cast<float>(i) / 36.0f * ds::PI * 2.0f);
	}
	for (int i = 0; i < 16; ++i) {
		settings.hTable[i] = ds::random(5.0f, 15.0f);
	}
	while (ds::isRunning()) {

		ds::begin();

		spriteBuffer.begin();

		int ret = showDialog(&settings);

		spriteBuffer.flush();
		
		ds::end();
	}
	ds::shutdown();
}