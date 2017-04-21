#pragma once
#include "..\..\..\diesel.h"

class SpriteBuffer;

struct Score {
	int itemsCleared;
	int seconds;
	int minutes;
	int points;
	int totalPoints;
};

class HUD {

public:
	HUD(SpriteBuffer* buffer, RID textureID, Score* score);
	~HUD();
	void render();
	void tick(float dt);
	void reset();
	void rebuildScore();
private:
	float _timer;
	int _numbers[6];
	int _minutes[2];
	int _seconds[2];
	SpriteBuffer* _buffer;
	RID _textureID;
	Score* _score;
};

