#include "utils.h"
#include "..\Constants.h"
#include <string.h>
#include "..\..\common\SpriteBuffer.h"

namespace font {

	void renderText(const v2& pos, const char* txt, SpriteBuffer* buffer, RID textureID) {
		int l = strlen(txt);
		v2 p = pos;
		for (int i = 0; i < l; ++i) {
			int idx = (int)txt[i] - 32;
			if (idx >= 0 && idx < 127) {
				const v4& r = FONT_RECTS[idx];
				buffer->add(p, textureID, r);
				p.x += r.z + 6.0f;
			}
		}
	}
}

namespace input {

	bool convertMouse2Grid(int* gridX, int* gridY) {
		v2 mousePos = ds::getMousePosition();
		float fy = (mousePos.y - static_cast<float>(STARTY)) / static_cast<float>(CELL_SIZE);
		float fx = (mousePos.x - static_cast<float>(STARTX - HALF_CELL_SIZE)) / static_cast<float>(CELL_SIZE);
		if (fx >= 0.0f && fy >= 0.0f) {
			int mx = static_cast<int>(fx);
			int my = static_cast<int>(fy);
			if (mx < MAX_X && my < MAX_Y) {
				*gridX = mx;
				*gridY = my;
				return true;
			}
		}
		return false;
	}
}