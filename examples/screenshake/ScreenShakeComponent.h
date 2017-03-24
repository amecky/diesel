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
private:
	ScreenShakeSettings* _settings;
	ScreenShakeBuffer _buffer;
};

