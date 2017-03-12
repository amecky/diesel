#pragma once
#include "..\..\diesel.h"

class SpriteBuffer;

namespace gui {

	void init(SpriteBuffer* buffer, RID textureID);

	void start(const ds::vec2& pos);

	void begin(const char* header, int* state);

	void beginGroup();

	void endGroup();

	bool Button(const char* text);

	void Text(const char* text);

	void Label(const char* label, const char* text);

	void Value(const char* label, const ds::vec2& v);

	void FormattedText(const char* fmt, ...);

	bool Input(const char* label, char* str, int maxLength);

	bool Input(const char* label, int* v);

	bool Input(const char* label, float* v);

	bool Input(const char* label, ds::vec2* v);

	bool Input(const char* label, ds::vec3* v);

	void StepInput(const char* label, int* v, int minValue, int maxValue, int steps);

	void Checkbox(const char* label, bool* state);

	void beginMenu();

	void MenuBar(const char* name, int* state);

	bool MenuItem(const char* label);

	void endMenu();

	void end();

	void shutdown();

}


