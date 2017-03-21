#include "HUD.h"
#include "..´\..\..\..\common\SpriteBuffer.h"

HUD::HUD(SpriteBuffer* buffer, RID textureID) : _buffer(buffer) , _textureID(textureID) {
	reset();
}

HUD::~HUD() {
}

// ------------------------------------------------------
// reset
// ------------------------------------------------------
void HUD::reset() {
	for (int i = 0; i < 6; ++i) {
		_numbers[i] = 0;
	}
	_minutes[0] = 0;
	_minutes[1] = 0;
	_seconds[0] = 0;
	_seconds[1] = 0;
	_timer = 0.0f;
	setNumber(0);
}

// ------------------------------------------------------
// tick
// ------------------------------------------------------
void HUD::tick(float dt) {
	_timer += dt;
	if (_timer > 1.0f) {
		_timer -= 1.0f;
		_seconds[1] += 1;
		if (_seconds[1] >= 10) {
			_seconds[1] = 0;
			_seconds[0] += 1;
			if (_seconds[0] >= 6) {
				_seconds[0] = 0;
				_minutes[1] += 1;
				if (_minutes[1] >= 10) {
					_minutes[1] = 0;
					_minutes[0] += 1;
				}
			}
		}
	}
}

// ------------------------------------------------------
// set number
// ------------------------------------------------------
void HUD::setNumber(int value) {
	int tmp = value;
	int div = 1;
	for (int i = 0; i < 6; ++i) {
		if (i > 0) {
			div *= 10;
		}
	}
	for (int i = 0; i < 6; ++i) {
		int r = tmp / div;
		_numbers[i] = r;
		tmp = tmp - r * div;
		div /= 10;
	}
}

// ------------------------------------------------------
// render
// ------------------------------------------------------
void HUD::render() {
	
	ds::vec2 p(103, 13);
	for (int i = 0; i < 6; ++i) {
		_buffer->add(p, ds::vec4(340 + _numbers[i] * 48, 0, 46, 42));
		p.x += 48.0f;
	}
	p.x = 725.0f;
	for (int i = 0; i < 2; ++i) {
		_buffer->add(p, ds::vec4(340 + _minutes[i] * 48, 0, 46, 42));
		p.x += 48.0f;
	}
	_buffer->add(p, ds::vec4(820, 0, 46, 42));
	p.x += 48.0f;
	for (int i = 0; i < 2; ++i) {
		_buffer->add(p, ds::vec4(340 + _seconds[i] * 48, 0, 46, 42));
		p.x += 48.0f;
	}
}
