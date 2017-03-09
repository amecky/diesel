#pragma once
#include "..\..\diesel.h"
#include "..\common\math.h"

namespace grid {

	v2 convertMousePos(const v2& mousePos);

	v2 convertMousePosToGridPos(const v2& mousePos);

	v2 convertFromGrid(int gx,int gy);
}

