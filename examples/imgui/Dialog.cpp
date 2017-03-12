#include "Dialog.h"
#include <vector>
#include "..\common\SpriteBuffer.h"
#include <stdarg.h>

namespace gui {

	typedef uint32_t HashedId;

	static HashedId NULL_HASH = 0;

	HashedId HashId(const char *id) {
		// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
		HashedId hash = 0x84222325;
		while (*id) hash = (hash ^ static_cast<uint8_t>(*id++)) * 0x000001b3;
		//assert(hash != NULL_HASH);
		return hash;
	}

	HashedId HashPointer(const void *ptr) {
		HashedId hash = static_cast<HashedId>(reinterpret_cast<size_t>(ptr)) * 2654435761;
		//assert(hash != NULL_HASH);
		return hash;
	}

	struct DrawCall {
		ds::vec2 pos;
		ds::vec2 size;
		ds::vec4 rect;
		ds::Color color;
		bool resize;
	};

	struct GUISettings {
		float textPadding;
		ds::Color backgroundColor;
		ds::Color inputBoxColor;
		ds::Color boxColor;
	};

	const ds::vec2 WHITE_RECT = ds::vec2(0, 128);

	struct GUIContext {
		RID textureID;
		SpriteBuffer* buffer;
		std::vector<DrawCall> calls;
		bool clicked;
		bool buttonPressed;
		int keyInput[256];
		int keys[32];
		int numKeys;
		ds::vec2 startPos;
		ds::vec2 currentPos;
		GUISettings settings;
		bool grouping;
		char inputText[32];
		char tmpBuffer[256];
		HashedId active;
		int caretPos;
		ds::vec2 size;
	};

	static GUIContext* _guiCtx = 0;

	// -------------------------------------------------------
	// check if mouse cursor is inside box
	// -------------------------------------------------------
	static bool isCursorInside(const ds::vec2& p, const ds::vec2& dim) {
		ds::vec2 mp = ds::getMousePosition();
		if (mp.x < (p.x - dim.x * 0.5f)) {
			return false;
		}
		if (mp.x >(p.x + dim.x * 0.5f)) {
			return false;
		}
		if (mp.y < (p.y - dim.y * 0.5f)) {
			return false;
		}
		if (mp.y >(p.y + dim.y * 0.5f)) {
			return false;
		}
		return true;
	}

	// -------------------------------------------------------
	// handle mouse interaction
	// -------------------------------------------------------
	static bool isClicked(const ds::vec2& pos, const ds::vec2& size) {
		if (_guiCtx->clicked) {
			ds::vec2 p = pos;
			p.x += size.x * 0.5f;
			if (_guiCtx->clicked && isCursorInside(p, size)) {
				return true;
			}
		}
		return false;
	}

	// -------------------------------------------------------
	// is box selected
	// -------------------------------------------------------
	static bool isBoxSelected(HashedId id, const ds::vec2& pos, const ds::vec2& size, bool setActive = true) {
		if (_guiCtx->clicked) {
			ds::vec2 p = pos;
			p.x += size.x * 0.5f;
			if (_guiCtx->clicked && isCursorInside(p, size)) {
				if (setActive) {
					_guiCtx->active = id;
					_guiCtx->clicked = false;
				}
				return true;
			}
		}
		return false;
	}

	// -------------------------------------------------------
	// determine text size
	// -------------------------------------------------------
	ds::vec2 textSize(const char* txt) {
		int l = strlen(txt);
		ds::vec2 p(0.0f);
		for (int i = 0; i < l; ++i) {
			int idx = (int)txt[i] - 32;
			if (idx >= 0 && idx < 127) {
				int c = idx / 16;
				int r = c - idx * 16;
				ds::vec4 rect = ds::vec4(r, c, 7, 10);
				p.x += rect.z + _guiCtx->settings.textPadding;
				if (rect.w > p.y) {
					p.y = rect.w;
				}
			}
		}
		return p;
	}

	// -------------------------------------------------------
	// determine text size
	// -------------------------------------------------------
	ds::vec2 limitedTextSize(const char* txt,int maxLength) {
		int l = strlen(txt);
		if (l > maxLength) {
			l = maxLength;
		}
		ds::vec2 p(0.0f);
		for (int i = 0; i < l; ++i) {
			int idx = (int)txt[i] - 32;
			if (idx >= 0 && idx < 127) {
				int c = idx / 16;
				int r = c - idx * 16;
				ds::vec4 rect = ds::vec4(r, c, 7, 10);
				p.x += rect.z + _guiCtx->settings.textPadding;
				if (rect.w > p.y) {
					p.y = rect.w;
				}
			}
		}
		return p;
	}

	// -------------------------------------------------------
	// initialize GUI
	// -------------------------------------------------------
	void init(SpriteBuffer* buffer, RID textureID) {
		_guiCtx = new GUIContext;
		_guiCtx->buffer = buffer;
		_guiCtx->textureID = textureID;
		_guiCtx->clicked = false;
		_guiCtx->buttonPressed = false;
		for (int i = 0; i < 255; ++i) {
			_guiCtx->keyInput[i] = 0;
		}
		_guiCtx->settings.textPadding = 2.0f;
		_guiCtx->settings.backgroundColor = ds::Color(56,69,74,255);
		_guiCtx->settings.boxColor = ds::Color(51, 86, 100, 255);
		_guiCtx->grouping = false;
		_guiCtx->numKeys = 0;
	}

	// --------------------------------------------------------
	// internal add text method
	// --------------------------------------------------------
	static void addText(const ds::vec2& pos, const char* text, float xoffset = 10) {
		int l = strlen(text);
		ds::vec2 p = pos;
		p.x += xoffset;
		for (int i = 0; i < l; ++i) {
			int idx = (int)text[i] - 32;
			if (idx >= 0 && idx < 127) {
				int c = idx / 16;
				int r = idx - c * 16;
				ds::vec4 rect = ds::vec4(r * 16, c * 16, 8, 14);
				DrawCall call;
				call.pos = p;
				call.rect = rect;
				call.color = ds::Color(1.0f, 1.0f, 1.0f, 1.0f);
				call.resize = false;
				_guiCtx->calls.push_back(call);
				p.x += rect.z + _guiCtx->settings.textPadding;
			}
		}
		if (p.x > _guiCtx->size.x) {
			_guiCtx->size.x = p.x;
		}
	}

	// --------------------------------------------------------
	// internal add texture box
	// --------------------------------------------------------
	static void addBox(const ds::vec2& p, const ds::vec2& size, const ds::Color& color, bool resize = false) {
		DrawCall call;
		call.pos = p;
		call.pos.x += size.x * 0.5f;
		call.rect = ds::vec4(256.0f, 0.0f, size.x, size.y);
		call.color = color;
		call.resize = resize;
		_guiCtx->calls.push_back(call);
		if ((p.x + size.x) > _guiCtx->size.x) {
			_guiCtx->size.x = (p.x + size.x);
		}
	}

	// --------------------------------------------------------
	// internal add texture box
	// --------------------------------------------------------
	static void addImage(const ds::vec2& p, const ds::vec4& rect) {
		DrawCall call;
		call.pos = p;
		call.pos.x += rect.z * 0.5f;
		call.rect = rect;
		call.color = ds::Color(255,255,255,255);
		call.resize = false;
		_guiCtx->calls.push_back(call);
		if ((p.x + rect.z) > _guiCtx->size.x) {
			_guiCtx->size.x = (p.x + rect.z);
		}
	}

	// --------------------------------------------------------
	// move current position to next position
	// --------------------------------------------------------
	static void moveForward(const ds::vec2& dim) {
		if (_guiCtx->grouping) {
			_guiCtx->currentPos.x += dim.x + 10.0f;
		}
		else {
			_guiCtx->currentPos.y -= dim.y;			
			_guiCtx->size.y += dim.y;
		}		
	}

	// -------------------------------------------------------
	// handle text input
	// -------------------------------------------------------
	// FIXME: provide numeric input only
	static bool handleTextInput(bool numeric = false) {
		bool ret = false;
		int len = strlen(_guiCtx->inputText);
		if ( _guiCtx->numKeys > 0 ) {
			for (int i = 0; i < _guiCtx->numKeys; ++i) {
				int current = _guiCtx->keys[i];
				if (current == 8) {
					if (_guiCtx->caretPos > 0) {
						if (_guiCtx->caretPos < len) {
							memmove(_guiCtx->inputText + _guiCtx->caretPos - 1, _guiCtx->inputText + _guiCtx->caretPos, len - _guiCtx->caretPos);
						}
						--_guiCtx->caretPos;
						--len;
						_guiCtx->inputText[len] = '\0';

					}
				}
				else if (current == 12) {
					if (_guiCtx->caretPos > 0) {
						--_guiCtx->caretPos;
					}
				}
				else if (current == 130) {
					if (_guiCtx->caretPos < strlen(_guiCtx->inputText)) {
						++_guiCtx->caretPos;
					}
				}
				else if (current == 131) {
					_guiCtx->caretPos = 0;
				}
				else if (current == 132) {
					_guiCtx->caretPos = strlen(_guiCtx->inputText);
				}
				else if (current == 13) {
					// return pressed
					_guiCtx->active = -1;
					ret = true;
				}
				else if (current == 134) {
					if (len > 0) {
						if (_guiCtx->caretPos < len) {
							memmove(_guiCtx->inputText + _guiCtx->caretPos, _guiCtx->inputText + _guiCtx->caretPos + 1, len - _guiCtx->caretPos);
							--len;
							_guiCtx->inputText[len] = '\0';
						}
					}
				}
				else if (current > 31 && current < 128) {
					if (len < 32) {
						if (_guiCtx->caretPos < len) {
							memmove(_guiCtx->inputText + _guiCtx->caretPos + 1, _guiCtx->inputText + _guiCtx->caretPos, len - _guiCtx->caretPos);
						}
						if (numeric) {
							if ((current > 47 && current < 58) || current == 46) {
								_guiCtx->inputText[_guiCtx->caretPos] = current;
								++len;
								++_guiCtx->caretPos;
							}
						}
						else {
							_guiCtx->inputText[_guiCtx->caretPos] = current;
							++len;
							++_guiCtx->caretPos;
						}
					}
				}
			}
			_guiCtx->inputText[len] = '\0';
		}
		//++len;		
		return ret;
	}
	// -------------------------------------------------------
	// input scalar
	// -------------------------------------------------------
	static bool InputScalar(int id, int index, char* v, int maxLength, float width = 150.0f) {
		int new_id = id + 1024 * index;
		bool ret = false;
		ds::vec2 p = _guiCtx->currentPos;
		//p.x += (width + 10.0f) * index + 150.0f;
		//bool hot = isHot(new_id, p, v2(width, BOX_HEIGHT), width * 0.5f);
		bool selected = isBoxSelected(new_id, p, ds::vec2(width, 20.0f));
		if (selected) {
			sprintf_s(_guiCtx->inputText, 32, "%s", v);
			//_guiCtx->caretPos = strlen(_guiCtx->inputText);
			_guiCtx->active = new_id;
		}
		if (_guiCtx->active == new_id) {
			addBox(p, ds::vec2(150.0f,20.0f),ds::Color(0,192,0,255));
			//ret = handleTextInput();
			strncpy(v, _guiCtx->inputText, maxLength);
			ds::vec2 cp = p;
			//ds::vec2 cursorPos = ds::font::calculateLimitedSize(guiContext->font, guiContext->inputText, guiContext->caretPos, CHAR_PADDING);
			//cp.x = _guiCtx->currentPos.x + 2.0f+ (width + 10.0f) * index + cursorPos.x + guiContext->settings[GS_LABELSIZE];
			//guiContext->addBox(cp, ds::vec2(2, BOX_HEIGHT - 4.0f), ds::Color(192, 0, 0, 255));
			p.y -= 1.0f;
			addText(p, _guiCtx->inputText);
			sprintf_s(v, maxLength, "%s", _guiCtx->inputText);
		}
		else {
			sprintf_s(_guiCtx->tmpBuffer, 64, "%s", v);
			addBox(p, ds::vec2(150.0f, 20.0f), ds::Color(192, 0, 0, 255));
			p.y -= 1.0f;
			addText(p, _guiCtx->tmpBuffer);
		}
		return ret;
	}

	// -------------------------------------------------------
	// internal input int
	// -------------------------------------------------------
	static bool InputScalar(int id, int index, int* v, float width = 100.0f) {
		int new_id = id + 1024 * index;
		bool ret = false;
		ds::vec2 p = _guiCtx->currentPos;
		bool selected = isBoxSelected(new_id, p, ds::vec2(width, 20.0f));
		if (selected) {
			sprintf_s(_guiCtx->inputText, 32, "%d", *v);
			_guiCtx->caretPos = strlen(_guiCtx->inputText);
			_guiCtx->active = new_id;
		}
		if (_guiCtx->active == new_id) {
			addBox(p, ds::vec2(width, 20.0f), ds::Color(0, 192, 0, 255));
			ret = handleTextInput(true);
			*v = atoi(_guiCtx->inputText);
			ds::vec2 cp = p;
			ds::vec2 cursorPos = limitedTextSize(_guiCtx->inputText, _guiCtx->caretPos);
			cp.x = _guiCtx->currentPos.x + 2.0f + (width + 10.0f) * index + cursorPos.x + 10.0f;
			addBox(cp, ds::vec2(2, 30.0f- 4.0f), ds::Color(192, 0, 192, 255));
			p.y -= 1.0f;
			addText(p, _guiCtx->inputText);
		}
		else {
			sprintf_s(_guiCtx->tmpBuffer, 64, "%d", *v);
			ds::vec2 textDim = textSize(_guiCtx->tmpBuffer);
			addBox(p, ds::vec2(width, 20.0f), ds::Color(192, 0, 0, 255));
			p.y -= 1.0f;
			p.x += (width - textDim.x) * 0.5f;
			addText(p, _guiCtx->tmpBuffer);
		}
		return ret;
	}

	// -------------------------------------------------------
	// internal input float
	// -------------------------------------------------------
	static bool InputScalar(int id, int index, float* v, float width = 100.0f) {
		int new_id = id + 1024 * index;
		bool ret = false;
		ds::vec2 p = _guiCtx->currentPos;
		if (index > 0) {
			p.x += index * width + index * 10.0f;
		}
		bool selected = isBoxSelected(new_id, p, ds::vec2(width, 20.0f));
		if (selected) {
			sprintf_s(_guiCtx->inputText, 32, "%g", *v);
			_guiCtx->caretPos = strlen(_guiCtx->inputText);
			_guiCtx->active = new_id;
		}
		if (_guiCtx->active == new_id) {
			addBox(p, ds::vec2(width, 20.0f), ds::Color(0, 192, 0, 255));
			ret = handleTextInput(true);
			*v = atof(_guiCtx->inputText);
			ds::vec2 cp = p;
			ds::vec2 cursorPos = limitedTextSize(_guiCtx->inputText, _guiCtx->caretPos);
			cp.x = _guiCtx->currentPos.x + 2.0f + (width + 10.0f) * index + cursorPos.x + 10.0f;
			addBox(cp, ds::vec2(2, 30.0f - 4.0f), ds::Color(192, 0, 192, 255));
			p.y -= 1.0f;
			addText(p, _guiCtx->inputText);
		}
		else {
			sprintf_s(_guiCtx->tmpBuffer, 64, "%g", *v);
			ds::vec2 textDim = textSize(_guiCtx->tmpBuffer);
			addBox(p, ds::vec2(width, 20.0f), ds::Color(192, 0, 0, 255));
			p.y -= 1.0f;
			p.x += (width - textDim.x) * 0.5f;
			addText(p, _guiCtx->tmpBuffer);
		}
		return ret;
	}

	// --------------------------------------------------------
	// begin
	// --------------------------------------------------------
	void start(const ds::vec2& pos) {
		_guiCtx->calls.clear();
		_guiCtx->startPos = pos;
		_guiCtx->currentPos = pos;
		_guiCtx->size = ds::vec2(0.0f, 0.0f);
		_guiCtx->grouping = false;
		for (int i = 0; i < 255; ++i) {
			if (ds::isKeyPressed(i) && _guiCtx->keyInput[i] == 0) {
				_guiCtx->keys[_guiCtx->numKeys++] = i;
				printf("pressed %d\n", i);
				_guiCtx->keyInput[i] = 1;
			}
			if (!ds::isKeyPressed(i) && _guiCtx->keyInput[i] == 1) {
				printf("released %d\n", i);
				_guiCtx->keyInput[i] = 0;
			}
		}
		if (_guiCtx->clicked) {
			_guiCtx->clicked = false;
		}
		if (ds::isMouseButtonPressed(0)) {
			_guiCtx->buttonPressed = true;
		}
		else {
			if (_guiCtx->buttonPressed) {
				_guiCtx->clicked = true;
			}
			_guiCtx->buttonPressed = false;
		}
	}

	// --------------------------------------------------------
	// begin with header
	// --------------------------------------------------------
	void begin(const char* header,int* state) {
		//begin(pos);
		ds::vec2 pos = _guiCtx->currentPos;
		addBox(pos, ds::vec2(10, 30), ds::Color(192, 0, 0, 255),true);
		pos.x += 30.0f;
		addText(pos, header);
		
		pos = _guiCtx->currentPos;
		pos.x -= 10.0f;
		addBox(pos, ds::vec2(30, 30), ds::Color(31, 56, 67, 255));
		if (*state == 0) {
			addText(_guiCtx->currentPos, "+");
		}
		else {
			addText(_guiCtx->currentPos, "-");
		}
		if (isClicked(_guiCtx->currentPos, ds::vec2(30, 30))) {
			if (*state == 0) {
				*state = 1;
			}
			else {
				*state = 0;
			}
		}
		
		moveForward(ds::vec2(10, 30));
	}

	// --------------------------------------------------------
	// begin grouping
	// --------------------------------------------------------
	void beginGroup() {
		_guiCtx->grouping = true;
	}

	// --------------------------------------------------------
	// end grouping
	// --------------------------------------------------------
	void endGroup() {
		_guiCtx->grouping = false;
		moveForward(ds::vec2(10, 30));
	}

	// --------------------------------------------------------
	// Button
	// --------------------------------------------------------
	bool Button(const char* text) {
		ds::vec2 p = _guiCtx->currentPos;
		addImage(p,ds::vec4(0.0f,96.0f,150.0f,24.0f));
		ds::vec2 dim = ds::vec2(150, 24);
		ds::vec2 textDim = textSize(text);
		p.x += (150.0f - textDim.x) * 0.5f;
		addText(p, text);
		dim.y = 30.0f;
		moveForward(dim);
		return isClicked(_guiCtx->currentPos, dim);
	}

	// --------------------------------------------------------
	// Button
	// --------------------------------------------------------
	void Checkbox(const char* label, bool* state) {
		ds::vec2 p = _guiCtx->currentPos;
		if (*state) {
			addBox(p, ds::vec2(40, 20), ds::Color(0, 192, 0, 255));
		}
		else {
			addBox(p, ds::vec2(40, 20), ds::Color(192, 0, 0, 255));
		}		
		if (*state) {
			p.x += 6;
			addText(p, "ON");
		}
		else {
			addText(p, "OFF");
		}
		if (isClicked(_guiCtx->currentPos, ds::vec2(40, 20))) {
			*state = !*state;
		}
		p = _guiCtx->currentPos;
		p.x += 50.0f;
		addText(p, label);
		moveForward(ds::vec2(40, 30));
	}

	// --------------------------------------------------------
	// Label
	// --------------------------------------------------------
	void Label(const char* label, const char* text) {
		addBox(_guiCtx->currentPos, ds::vec2(150.0f,20.0f),_guiCtx->settings.boxColor);
		ds::vec2 textDim = textSize(text);

		ds::vec2 textPos = _guiCtx->currentPos;
		textPos.x += (160.0f - textDim.x) * 0.5f;
		addText(textPos, text);

		ds::vec2 labelPos = _guiCtx->currentPos;
		labelPos.x += 160.0f;
		addText(labelPos, label);
		ds::vec2 size = textSize(label);
		moveForward(ds::vec2(size.x, 30.0f));
	}

	// --------------------------------------------------------
	// Value - vec2
	// --------------------------------------------------------
	void Value(const char* label, const ds::vec2& v) {
		sprintf_s(_guiCtx->tmpBuffer, 256, "%g %g", v.x, v.y);
		Label(label, _guiCtx->tmpBuffer);
	}

	// --------------------------------------------------------
	// Text
	// --------------------------------------------------------
	void Text(const char* text) {
		int l = strlen(text);
		ds::vec2 p = _guiCtx->currentPos;
		addText(p, text);
		ds::vec2 size = textSize(text);
		moveForward(ds::vec2(size.x, 30.0f));
	}

	// --------------------------------------------------------
	// Formatted text
	// --------------------------------------------------------
	void FormattedText(const char* fmt, ...) {
		char buffer[1024];
		va_list args;
		va_start(args, fmt);
		vsprintf(buffer, fmt, args);
		Text(buffer);
		va_end(args);
		
	}

	// -------------------------------------------------------
	// input string
	// -------------------------------------------------------
	bool Input(const char* label, char* str, int maxLength) {
		HashedId id = HashPointer(str);
		ds::vec2 p = _guiCtx->currentPos;
		p.x += 160.0f;
		addText(p, label);
		bool ret = InputScalar(id, 0, str, maxLength, 150.0f);
		moveForward(ds::vec2(150.0f, 30.0f));
		return ret;
	}

	// -------------------------------------------------------
	// input int
	// -------------------------------------------------------
	bool Input(const char* label, int* v) {
		HashedId id = HashPointer(v);
		ds::vec2 p = _guiCtx->currentPos;
		p.x += 160.0f;
		addText(p, label);
		bool ret = InputScalar(id, 0, v, 150.0f);
		ds::vec2 ts = textSize(label);
		moveForward(ds::vec2(150.0f + ts.x + 10.0f, 30.0f));
		return ret;
	}

	// -------------------------------------------------------
	// input float
	// -------------------------------------------------------
	bool Input(const char* label, float* v) {
		HashedId id = HashPointer(v);
		ds::vec2 p = _guiCtx->currentPos;
		p.x += 160.0f;
		addText(p, label);
		bool ret = InputScalar(id, 0, v, 150.0f);
		ds::vec2 ts = textSize(label);
		moveForward(ds::vec2(150.0f + ts.x + 10.0f, 30.0f));
		return ret;
	}

	// -------------------------------------------------------
	// input vec2
	// -------------------------------------------------------
	bool Input(const char* label, ds::vec2* v) {
		HashedId id = HashPointer(v);
		ds::vec2 p = _guiCtx->currentPos;
		p.x += 160.0f;
		addText(p, label);
		InputScalar(id, 0, &v->x, 70.0f);
		bool ret = InputScalar(id, 1, &v->y, 70.0f);
		ds::vec2 ts = textSize(label);
		moveForward(ds::vec2(150.0f + ts.x + 10.0f, 30.0f));
		return ret;
	}

	// -------------------------------------------------------
	// input vec3
	// -------------------------------------------------------
	bool Input(const char* label, ds::vec3* v) {
		HashedId id = HashPointer(v);
		ds::vec2 p = _guiCtx->currentPos;
		p.x += 240.0f;
		addText(p, label);
		InputScalar(id, 0, &v->x, 70.0f);
		InputScalar(id, 1, &v->y, 70.0f);
		InputScalar(id, 2, &v->z, 70.0f);
		ds::vec2 ts = textSize(label);
		moveForward(ds::vec2(150.0f + ts.x + 10.0f, 30.0f));
		return true;
	}

	// -------------------------------------------------------
	// int input using steps
	// -------------------------------------------------------
	void StepInput(const char* label, int* v, int minValue, int maxValue, int steps) {
		HashedId id = HashPointer(v);
		ds::vec2 p = _guiCtx->currentPos;
		// -
		addBox(p, ds::vec2(20, 20), ds::Color(31, 56, 67, 255));
		if (isClicked(p, ds::vec2(20, 20))) {
			*v -= steps;
			if (*v < minValue) {
				*v = minValue;
			}
		}
		addText(p, "-");
		// value
		sprintf_s(_guiCtx->tmpBuffer, 256, "%d", *v);
		ds::vec2 textDim = textSize(_guiCtx->tmpBuffer);
		p = _guiCtx->currentPos;
		p.x += 30.0f;
		addBox(p, ds::vec2(90, 20), ds::Color(31, 56, 67, 255));
		p.x += (90.0f - textDim.x) * 0.5f;
		addText(p, _guiCtx->tmpBuffer);
		// +
		p = _guiCtx->currentPos;
		p.x += 130.0f;
		addBox(p, ds::vec2(20, 20), ds::Color(31, 56, 67, 255));
		if (isClicked(p, ds::vec2(20, 20))) {
			*v += steps;
			if (*v > maxValue) {
				*v = maxValue;
			}
		}
		addText(p, "+");
		// label
		p.x += 30.0f;
		addText(p, label);
		ds::vec2 ts = textSize(label);
		moveForward(ds::vec2(150.0f + ts.x + 30.0f, 30.0f));
	}

	// --------------------------------------------------------
	// end - render all draw calls
	// --------------------------------------------------------
	void end() {
		float h = _guiCtx->size.y - 10.0f;
		float sy = h / 128.0f;
		float w = _guiCtx->size.x + 20.0f;
		float sx = w / 128.0f;
		float bpx = _guiCtx->startPos.x + w * 0.5f;
		float bpy = _guiCtx->startPos.y - h * 0.5f;

		_guiCtx->buffer->add(ds::vec2(bpx,bpy), _guiCtx->textureID, ds::vec4(256,0,128,128),ds::vec2(sx,sy),0.0f,_guiCtx->settings.backgroundColor);

		for (size_t i = 0; i < _guiCtx->calls.size(); ++i) {
			const DrawCall& call = _guiCtx->calls[i];
			ds::vec2 p = call.pos;
			if (call.resize) {
				ds::vec4 rect = call.rect;
				rect.z = 128.0f;
				p.x = bpx;
				_guiCtx->buffer->add(p, _guiCtx->textureID, rect, ds::vec2(sx, 1.0f), 0.0f, call.color);
			}
			else {
				p.x += 10.0f;
				_guiCtx->buffer->add(p, _guiCtx->textureID, call.rect, ds::vec2(1.0f, 1.0f), 0.0f, call.color);
			}
		}
		_guiCtx->numKeys = 0;
	}

	// --------------------------------------------------------
	// shutdown GUI
	// --------------------------------------------------------
	void shutdown() {
		if (_guiCtx != 0) {
			delete _guiCtx;
		}
	}

}
