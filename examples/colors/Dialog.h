#pragma once
#include "..\..\diesel.h"

class SpriteBuffer;

namespace gui {

	void init(SpriteBuffer* buffer, RID textureID);

	void begin();

	void Image(const ds::vec2& pos, const ds::vec4& rect);

	bool Button(const ds::vec2& pos, const ds::vec4& rect);

	void Text(const ds::vec2& pos, const char* text);

	void FormattedText(const ds::vec2& pos, const char* fmt, ...);

	void end();

	void shutdown();

}


