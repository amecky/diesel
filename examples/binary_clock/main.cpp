#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\SpriteBuffer.h"
#include "..\common\imgui.h"

struct TimeCol {
	int state[4];
	int num;
};

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

void convert(TimeCol* columns, int index, int value) {
	int upper = value / 10;
	int lower = value - upper * 10;
	if (value < 10) {
		lower = value;
		upper = 0;
	}
	for (int i = 0; i < 4; ++i) {
		int r = 0;
		if (upper & (1 << i)) {
			r = 1;
		}
		columns[index].state[i] = r;
		r = 0;
		if (lower & ( 1 << i)) {
			r = 1;
		}
		columns[index + 1].state[i] = r;
	}
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	//
	// prepare application
	//
	ds::RenderSettings rs;
	rs.width = 800;
	rs.height = 600;
	rs.title = "Binary clock";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);

	RID textureID = loadImage("TextureArray.png");
	SpriteBuffer spriteBuffer(2048, textureID);

	TimeCol columns[6];
	for (int i = 0; i < 6; ++i) {
		TimeCol& col = columns[i];
		col.num = 4;
		col.state[0] = 0;
		col.state[1] = 1;
		col.state[2] = 0;
		col.state[3] = 2;
	}

	SYSTEMTIME lt;

	GetLocalTime(&lt);
	convert(columns, 0, lt.wHour);
	convert(columns, 2, lt.wMinute);
	convert(columns, 4, lt.wSecond);

	columns[0].num = 2;
	columns[2].num = 3;
	columns[4].num = 3;

	while (ds::isRunning()) {

		ds::begin();

		GetLocalTime(&lt);

		convert(columns, 0, lt.wHour);
		convert(columns, 2, lt.wMinute);
		convert(columns, 4, lt.wSecond);

		spriteBuffer.begin();
		int xp = 185;
		for (int j = 0; j < 6; ++j) {
			const TimeCol& col = columns[j];
			xp += 50;
			if (j % 2 == 0) {
				xp += 20;
			}
			for (int i = 0; i < col.num; ++i) {
				if (col.state[i] == 0) {
					spriteBuffer.add(ds::vec2(xp, 220 + i * 50), ds::vec4(0, 0, 48, 48));
				}
				else {
					spriteBuffer.add(ds::vec2(xp, 220 + i * 50), ds::vec4(48, 0, 48, 48));
				}
			}
		}
		spriteBuffer.flush();
		
		ds::end();
	}
	ds::shutdown();
}