#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\SpriteBuffer.h"
#include "..\common\imgui.h"
#include "APath.h"
#include "FlowField.h"

struct Walker {
	ds::vec2 points[64];
	int numPoints;
	int index;
	ds::vec2 velocity;
	ds::vec2 pos;
	float rotation;
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

void resetWalker(Walker* walker, p2i* points, int num, int sx, int sy) {
	walker->numPoints = 0;
	for (int i = num - 1; i >= 0; --i) {
		p2i wp = points[i];
		walker->points[walker->numPoints++] = ds::vec2(sx + wp.x * 46, sy + 46 * wp.y);
	}
	walker->pos = walker->points[0];
	walker->velocity = (walker->points[1] - walker->points[0]) * 2.0f;
	walker->index = 0;
	walker->rotation = getAngle(walker->points[walker->index], walker->points[walker->index + 1]);	
}

bool moveWalker(Walker* walker, float dt) {
	walker->pos += walker->velocity * static_cast<float>(ds::getElapsedSeconds());
	ds::vec2 next = walker->points[walker->index + 1];
	ds::vec2 diff = walker->pos - next;
	if (sqr_length(diff) < 2.0f) {
		++walker->index;
		if (walker->index >= (walker->numPoints - 1)) {
			walker->index = 0;
		}
		walker->pos = walker->points[walker->index];
		walker->velocity = (walker->points[walker->index + 1] - walker->points[walker->index]) * 2.0f;
		walker->rotation = getAngle(walker->points[walker->index], walker->points[walker->index + 1]);
	}
	return true;
}

const ds::vec4 NUMBERS[] = {
	ds::vec4(0,46,17,14),
	ds::vec4(17,46,9,14),
	ds::vec4(26,46,16,14),
	ds::vec4(42,46,16,14),
	ds::vec4(58,46,17,14),
	ds::vec4(75,46,17,14),
	ds::vec4(92,46,17,14),
	ds::vec4(109,46,17,14),
	ds::vec4(126,46,17,14),
	ds::vec4(143,46,16,14),
};

void drawNumber(SpriteBuffer* buffer, int value, ds::vec2 p) {
	if (value < 10) {
		buffer->add(p, NUMBERS[value]);
	}
	else if (value < 100) {
		int f = value / 10;
		int s = value - f * 10;
		ds::vec4 ft = NUMBERS[f];
		ds::vec4 st = NUMBERS[s];
		float w = (ft.z + st.z) * 0.5f;
		p.x -= w;
		buffer->add(p, ft);
		p.x += ft.z + 2;
		buffer->add(p, st);
	}
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
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
int main(int argc, char *argv[]) {
	//
	// prepare application
	//
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "A*Path sandbox";
	rs.clearColor = ds::Color(0.1f, 0.1f, 0.1f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	ds::init(rs);
	
	RID guiTextureID = loadImage("..\\common\\imgui.png");
	RID textureID = loadImage("TextureArray.png");

	SpriteBuffer spriteBuffer(2048, textureID);

	//
	// prepare IMGUI
	//
	SpriteBuffer guiBuffer(512, guiTextureID);
	gui::init(&guiBuffer, guiTextureID);

	int state = 1;

	Grid grid(20,14);
	grid.set(3, 1, 1);
	grid.set(3, 2, 1);
	grid.set(3, 3, 1);
	grid.setStart(0, 2);
	grid.setEnd(19, 2);

	int totalX = grid.width * 46;
	int totalY = grid.height * 46;
	int sx = (1024 - totalX) / 2;
	int sy = (768 - totalY) / 2;

	APath ap(&grid);
	p2i points[64];

	FlowField flowField = FlowField(&grid);
	flowField.build(p2i(19,11));

	int num = ap.find(grid.start, grid.end, points, 64);

	ds::vec2 start = ds::vec2(0, 2);
	ds::vec2 end = ds::vec2(19, 2);

	bool clicked = false;
	bool pressed = false;

	int gx = 0;
	int gy = 0;

	Walker walker;
	resetWalker(&walker, points, num, sx, sy);

	while (ds::isRunning()) {

		if (ds::isMouseButtonPressed(0)) {
			pressed = true;
		}
		else if (pressed) {
			pressed = false;
			clicked = true;
		}

		ds::begin();

		

		if (clicked) {
			ds::vec2 mp = ds::getMousePosition();
			if (mp.x >= (sx - 23) && mp.y >= (sy - 23)) {
				gx = (mp.x - sx + 23) / 46;
				gy = (mp.y - sy + 23) / 46;
				int idx = gx + gy * grid.width;
				if (grid.items[idx] == 1) {
					grid.items[idx] = 0;
				}
				else {
					grid.items[idx] = 1;
				}
			}
			else {
				gx = -1;
				gy = -1;
			}
			clicked = false;
		}

		moveWalker(&walker,static_cast<float>(ds::getElapsedSeconds()));

		spriteBuffer.begin();
		for (int y = 0; y < grid.height; ++y) {
			for (int x = 0; x < grid.width; ++x) {
				ds::vec2 p = ds::vec2(sx + x * 46, sy + 46 * y);
				ds::vec4 t = ds::vec4(0, 0, 46, 46);
				if (grid.items[x + y * grid.width] == 1) {
					t = ds::vec4(46, 0, 46, 46);
				}
				else if (grid.items[x + y * grid.width] == 2) {
					t = ds::vec4(138, 0, 46, 46);
				}
				if (grid.items[x + y * grid.width] == 3) {
					t = ds::vec4(184, 0, 46, 46);
				}
				spriteBuffer.add(p, t);
				ds::vec2 ffp = flowField.get(x, y);
				drawNumber(&spriteBuffer, ffp.x, p);
			}
		}
		/*
		for (int i = 0; i < num; ++i) {
			p2i wp = points[i];
			ds::vec2 p = ds::vec2(sx + wp.x * 46, sy + 46 * wp.y);
			drawNumber(&spriteBuffer, i, p);
		}

		spriteBuffer.add(walker.pos, ds::vec4(184, 92, 24, 24), ds::vec2(1, 1), walker.rotation);
		*/
		spriteBuffer.flush();
		/*
		// GUI
		guiBuffer.begin();
		gui::start(ds::vec2(0, 750));
		gui::begin("Grid", &state);
		if (state == 1) {
			gui::FormattedText("Grid x: %d y: %d", gx, gy);
			gui::Input("Start", &start);
			gui::Input("End", &end);
			if (gui::Button("Generate")) {
				grid.setStart(start.x, start.y);
				grid.setEnd(end.x, end.y);
				num = ap.find(p2i(start.x, start.y), p2i(end.x,end.y), points, 64);
				resetWalker(&walker, points, num, sx, sy);
			}
		}
		gui::end();
		guiBuffer.flush();
		*/
		ds::end();
	}
	ds::shutdown();
}