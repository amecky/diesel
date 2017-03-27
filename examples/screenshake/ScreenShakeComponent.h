#pragma once
#include "..\..\diesel.h"

struct ScreenShakeSettings {
	float amplitude;
	float frequency;
	float ttl;
	RID renderTarget;
	RID finalPass;
};

struct ScreenShakeBuffer {
	float amplitude;
	float frequency;
	float ttl;
	float timer;
};

class ScreenShakeComponent {

public:
	ScreenShakeComponent(ScreenShakeSettings* settings);
	~ScreenShakeComponent();
	void tick(float elapsed);
	void render();
	bool isActive() const {
		return _active;
	}
	void activate();
private:
	float _timer;
	bool _active;
	RID _orthoPass;
	RID _screenShakeItem;
	ScreenShakeSettings* _settings;
	ScreenShakeBuffer _buffer;
};

