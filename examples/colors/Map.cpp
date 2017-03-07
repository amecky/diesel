#include "Map.h"
#include "Constants.h"

namespace grid {

	v2 convertMousePos(const v2& mousePos) {
		int my = 768 - (int)mousePos.y;	
		my = my - STARTY + HALF_CELL_SIZE;
		int mx = (int)mousePos.x - STARTX + HALF_CELL_SIZE;
		return v2(mx,my);
	}


	v2 convertMousePosToGridPos(const v2& mousePos) {
		v2 sp = convertMousePos(mousePos);
		int x = sp.x / CELL_SIZE;
		int y = sp.y / CELL_SIZE;
		return v2(x,y);
	}

	v2 convertFromGrid(int gx,int gy) {
		return v2(STARTX + gx * CELL_SIZE,STARTY + gy * CELL_SIZE);	
	}
}