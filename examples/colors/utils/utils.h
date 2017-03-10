#pragma once
#include "..\..\..\diesel.h"

class SpriteBuffer;

namespace font {

	void renderText(const ds::vec2& pos, const char* txt, SpriteBuffer* buffer, RID textureID);
}

namespace input {

	bool convertMouse2Grid(int* gridX, int* gridY);
}