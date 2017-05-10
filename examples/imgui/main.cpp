#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#define SPRITE_IMPLEMENTATION
#include "..\common\SpriteBatchBuffer.h"
#include "..\common\imgui.h"

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
	int menu;
	int diagramState;
	int listIndex;
	int listOffset;
	int valueState;
	int dropState;
	int dopIndex;
	int dropOffset;
	float angle;
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
const char* ITEMS[] = { "First","Second","Third","Fourth" };
// ---------------------------------------------------------------
// show game over menu
// ---------------------------------------------------------------
int showDialog(TestSettings* settings) {
	int ret = 0;
	gui::start(ds::vec2(0, 750));
	/*
	gui::beginMenu();
	//gui::MenuBar(ITEMS, 4, &settings->menu);
	if (gui::MenuBar("First")) {
		settings->menu = 1;
	}
	if (gui::MenuBar("Second")) {
		settings->menu = 2;
	}
	if (gui::MenuBar("Third")) {
		settings->menu = 3;
	}
	if (gui::MenuBar("Fourth")) {
		settings->menu = 4;
	}
	*/
	gui::begin("Basic elements",&settings->state);
	if (settings->state == 1) {
		ds::vec2 mp = ds::getMousePosition();
		gui::Value("Mouse Position", mp);
		gui::Value("Menu", settings->menu);
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
		gui::ListBox("Listbox", ITEMS, 4, &settings->listIndex, &settings->listOffset, 3);
		gui::Slider("Slider", &settings->iv, 0, 200,100.0f);
		gui::SliderAngle("Angle Slider", &settings->angle);
		gui::DropDownBox("Dropdown", ITEMS, 4, &settings->dropState, &settings->dopIndex, &settings->dropOffset, 3, true);
	}
	gui::begin("Value example", &settings->valueState);
	if (settings->valueState == 1) {
		gui::Value("Int", 200);
		gui::Value("Float", 123.0f);
		gui::Value("Vec2", ds::vec2(100.0f, 200.0f));
		gui::Value("Vec3", ds::vec3(12.0f, 34.0f, 56.0f));
		gui::Value("Vec4", ds::vec4(10, 20, 50, 60));
		gui::Value("Color", ds::Color(192, 32, 64, 128));
	}
	gui::begin("Diagrams", &settings->diagramState);
	if ( settings->diagramState == 1) {
		gui::Histogram(settings->hTable, 16, 0.0f, 20.0f, 5.0f, 300.0f,200.0f);
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
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
//int main(int argc, char *argv[]) {
	
	initialize();

	
	// load image using stb_image
	int x, y, n;
	unsigned char *data = stbi_load("..\\common\\imgui.png", &x, &y, &n, 4);
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE };
	RID textureID = ds::createTexture(texInfo);
	stbi_image_free(data);

	SpriteBatchBufferInfo sbbInfo = { 512, textureID };
	SpriteBatchBuffer spriteBuffer(sbbInfo);

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
	settings.menu = -1;
	settings.diagramState = 0;
	settings.listIndex = -1;
	settings.listOffset = 0;
	settings.valueState = 0;
	settings.dropState = 0;
	settings.dopIndex = -1;
	settings.dropOffset = 0;
	settings.angle = ds::PI;
	while (ds::isRunning()) {

		ds::begin();

		spriteBuffer.begin();

		int ret = showDialog(&settings);

		spriteBuffer.flush();
		
		ds::end();
	}
	ds::shutdown();
}