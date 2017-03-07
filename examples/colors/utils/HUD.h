#pragma once
#include "..\..\..\diesel.h"

class SpriteBuffer;

class HUD {

public:
	HUD(SpriteBuffer* buffer, RID textureID);
	~HUD();
	void render();
	void tick(float dt);
	void reset();
	void setNumber(int value);
private:
	float _timer;
	int _numbers[6];
	int _minutes[2];
	int _seconds[2];
	SpriteBuffer* _buffer;
	RID _textureID;
};

