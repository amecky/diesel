#include "Map.h"
#include "Constants.h"

namespace grid {

	ds::vec2 convertMousePos(const ds::vec2& mousePos) {
		int my = 768 - (int)mousePos.y;	
		my = my - STARTY + HALF_CELL_SIZE;
		int mx = (int)mousePos.x - STARTX + HALF_CELL_SIZE;
		return ds::vec2(mx,my);
	}


	ds::vec2 convertMousePosToGridPos(const ds::vec2& mousePos) {
		ds::vec2 sp = convertMousePos(mousePos);
		int x = sp.x / CELL_SIZE;
		int y = sp.y / CELL_SIZE;
		return ds::vec2(x,y);
	}

	ds::vec2 convertFromGrid(int gx,int gy) {
		return ds::vec2(STARTX + gx * CELL_SIZE,STARTY + gy * CELL_SIZE);	
	}
}