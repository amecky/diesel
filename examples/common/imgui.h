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

	void Value(const char* label, int v);

	void Value(const char* label, float v);

	void Value(const char* label, const ds::vec2& v);

	void Value(const char* label, const ds::vec3& v);

	void Value(const char* label, const ds::vec4& v);

	void Value(const char* label, const ds::Color& v);

	void FormattedText(const char* fmt, ...);

	bool Input(const char* label, char* str, int maxLength);

	void Input(const char* label, int* v);

	void Input(const char* label, float* v);

	void Input(const char* label, ds::vec2* v);

	void Input(const char* label, ds::vec3* v);

	void Input(const char* label, ds::Color* v);

	void StepInput(const char* label, int* v, int minValue, int maxValue, int steps);

	void Checkbox(const char* label, bool* state);

	void ListBox(const char* label, const char** entries, int num, int* selected, int *offset, int max);

	void DropDownBox(const char* label, const char** entries, int num, int* state, int* selected, int *offset, int max, bool closeOnSelection = false);

	void Slider(const char* label, int* v, int minValue, int maxValue, float width = 200.0f);

	void SliderAngle(const char* label, float* v, float width = 200.0f);

	void Separator();

	void Histogram(float* values, int num, float minValue, float maxValue, float step, float width = 200.0f, float height = 100.0f);

	void Diagram(float* values, int num, float minValue, float maxValue, float step, float width = 200.0f, float height = 100.0f);

	void Diagram(const char* label, float* values, int num, float minValue, float maxValue, float step, float width = 200.0f, float height = 100.0f);

	void beginMenu();

	bool MenuBar(const char* name);

	bool MenuItem(const char* label);

	void MenuBar(const char** names, int num, int* state);

	void endMenu();

	void end();

	void shutdown();

}


