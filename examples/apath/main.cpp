#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#include "..\common\SpriteBuffer.h"
#include "..\common\imgui.h"
#include "FlowField.h"
#include <stdio.h>

const static int START_X = 52;
const static int START_Y = 62;

struct Walker {
	p2i gridPos;
	ds::vec2 velocity;
	ds::vec2 pos;
	float rotation;
};

typedef std::vector<Walker> Walkers;

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
// convert screen coordinates to grid position if possible
// ---------------------------------------------------------------
bool convert(int screenX, int screenY, int startX, int startY, p2i* ret) {
	if (screenX >= (startX - 23) && screenY >= (startY - 23)) {
		ret->x = (screenX - startX + 23) / 46;
		ret->y = (screenY - startY + 23) / 46;
		return true;
	}
	return false;
}

// ---------------------------------------------------------------
// prepare walker
// ---------------------------------------------------------------
void prepareWalker(Walker* walker, const p2i& start) {
	walker->gridPos = start;
	walker->pos = ds::vec2(START_X + start.x * 46, START_Y + start.y * 46);
	walker->rotation = 0.0f;
	walker->velocity = ds::vec2(0.0f);
}

// ---------------------------------------------------------------
// move walker
// ---------------------------------------------------------------
bool moveWalker(FlowField* flowField, Walker* walker) {
	if (flowField->hasNext(walker->gridPos)) {
		p2i n = flowField->next(walker->gridPos);
		p2i nextPos = p2i(START_X + n.x * 46, START_Y + n.y * 46);
		ds::vec2 diff = walker->pos - ds::vec2(nextPos.x, nextPos.y);
		if (sqr_length(diff) < 4.0f) {
			convert(walker->pos.x, walker->pos.y, START_X, START_Y, &walker->gridPos);
		}
		ds::vec2 v = normalize(ds::vec2(nextPos.x, nextPos.y) - walker->pos) * 100.0f;
		walker->pos += v * static_cast<float>(ds::getElapsedSeconds());
		walker->rotation = getAngle(walker->pos, ds::vec2(nextPos.x, nextPos.y));
		return true;
	}
	return false;
}

// ---------------------------------------------------------------
// the texture coordinates for all numbers 0 - 9
// ---------------------------------------------------------------
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

// ---------------------------------------------------------------
// draw number
// ---------------------------------------------------------------
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
	ds::TextureInfo texInfo = { x, y, n, data, ds::TextureFormat::R8G8B8A8_UNORM , ds::BindFlag::BF_SHADER_RESOURCE};
	RID textureID = ds::createTexture(texInfo);
	stbi_image_free(data);
	return textureID;
}

void readGridData(Grid* grid, p2i* start, p2i* end) {
	FILE* fp = fopen("field.txt", "r");
	char* data = 0;
	int fileSize = -1;
	if (fp) {
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		data = new char[fileSize + 1];
		fread(data, 1, fileSize, fp);
		data[fileSize] = '\0';
		fclose(fp);
	}
	if (data != 0) {
		int idx = 0;
		for (int y = 13; y >= 0; --y) {
			for (int x = 0; x < 20; ++x) {
				if (data[idx] == 'x') {
					grid->set(x, y, 1);
				}
				if (data[idx] == '=') {
					grid->set(x, y, 4);
				}
				if (data[idx] == 's') {
					*start = p2i(x, y);
				}
				if (data[idx] == 'e') {
					*end = p2i(x, y);
				}
				++idx;
			}
			++idx;
		}
	}
	delete[] data;
}

// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
//int main(int argc, char *argv[]) {
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
	rs.supportDebug = true;
	ds::init(rs);
	
	RID textureID = loadImage("TextureArray.png");

	SpriteBuffer spriteBuffer(2048, textureID);
	
	int state = 1;

	Grid grid(20,14);
	p2i start(0, 0);
	p2i end(0, 0);
	readGridData(&grid, &start, &end);
	grid.setStart(start.x,start.y);
	grid.setEnd(end.x,end.y);

	FlowField flowField = FlowField(&grid);
	flowField.build(end);

	bool clicked = false;
	bool pressed = false;

	int gx = 0;
	int gy = 0;

	Walkers walkers;

	Walker walker;
	prepareWalker(&walker, start);
	walkers.push_back(walker);

	float step = ds::PI * 0.25f;

	//p2i current(0, 0);

	while (ds::isRunning()) {

		if (ds::isMouseButtonPressed(0)) {
			pressed = true;
		}
		else if (pressed) {
			pressed = false;
			clicked = true;
		}

		ds::begin();
		//
		// start a new walker
		//
		if (clicked) {
			ds::vec2 mp = ds::getMousePosition();
			if (mp.x >= (START_X - 23) && mp.y >= (START_Y - 23)) {
				gx = (mp.x - START_X + 23) / 46;
				gy = (mp.y - START_Y + 23) / 46;
				Walker walker;
				prepareWalker(&walker, p2i(gx, gy));
				walkers.push_back(walker);
			}
			else {
				gx = -1;
				gy = -1;
			}
			clicked = false;
		}
		//
		// move all walkers
		//
		Walkers::iterator it = walkers.begin();
		while (it != walkers.end()) {
			if (moveWalker(&flowField, &(*it))) {
				++it;
			}
			else {
				it = walkers.erase(it);
			}
		}
		//
		// draw grid
		//
		spriteBuffer.begin();
		for (int y = 0; y < grid.height; ++y) {
			for (int x = 0; x < grid.width; ++x) {
				ds::vec2 p = ds::vec2(START_X + x * 46, START_Y + 46 * y);
				ds::vec4 t = ds::vec4(0, 0, 46, 46);
				int type = grid.items[x + y * grid.width];
				if ( type == 1) {
					t = ds::vec4(46, 0, 46, 46);
				}
				else if (type == 2) {
					t = ds::vec4(138, 0, 46, 46);
				}
				if (type == 3) {
					t = ds::vec4(184, 0, 46, 46);
				}
				if (type != 4) {
					spriteBuffer.add(p, t);
					// draw direction
					int d = flowField.get(x, y);
					if (d >= 0 && d < 9) {
						spriteBuffer.add(p, ds::vec4(d * 46, 138, 46, 46));
					}
				}
			}
		}
		//
		// draw walkers
		//
		it = walkers.begin();
		while (it != walkers.end()) {
			spriteBuffer.add(it->pos, ds::vec4(276, 0, 24, 24), ds::vec2(1, 1), it->rotation);
			++it;
		}
		spriteBuffer.flush();
		ds::end();
	}
	ds::shutdown();
}