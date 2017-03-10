#pragma once
#include "..\..\diesel.h"

namespace grid {

	ds::vec2 convertMousePos(const ds::vec2& mousePos);

	ds::vec2 convertMousePosToGridPos(const ds::vec2& mousePos);

	ds::vec2 convertFromGrid(int gx,int gy);
}

