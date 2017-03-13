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

	void Input(const char* label, int* v);

	void Input(const char* label, float* v);

	void Input(const char* label, ds::vec2* v);

	void Input(const char* label, ds::vec3* v);

	void Input(const char* label, ds::Color* v);

	void StepInput(const char* label, int* v, int minValue, int maxValue, int steps);

	void Checkbox(const char* label, bool* state);

	void Separator();

	void Histogram(float* values, int num, float minValue, float maxValue, float step);

	void Diagram(float* values, int num, float minValue, float maxValue, float step);

	void Diagram(const char* label, float* values, int num, float minValue, float maxValue, float step);

	void beginMenu();

	void MenuBar(const char* name, int* state);

	bool MenuItem(const char* label);

	void endMenu();

	void end();

	void shutdown();

}


