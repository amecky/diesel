#pragma once
#include "Grid.h"
#include "..\..\diesel.h"
#include <vector>
#include "Constants.h"

class SpriteBuffer;
struct GameSettings;

// -------------------------------------------------------
// Color grid
// -------------------------------------------------------
enum TileState {
	TS_NORMAL,
	TS_WIGGLE,
	TS_SHRINKING
};

struct MyEntry {

	v4 texture;
	int color;
	bool hidden;
	float scale;
	float timer;
	TileState state;
};

struct MovingCell {

	int x;
	int y;
	v2 start;
	v2 end;
	v2 current;
	v4 texture;
};

class ColorGrid : public ds::Grid<MyEntry> {

public:
	ColorGrid() : ds::Grid<MyEntry>(MAX_X, MAX_Y) {}
	virtual ~ColorGrid() {}
	bool isMatch(const MyEntry& first, const MyEntry& right) {
		return first.color == right.color;
	}
};

//class MovingCells;
// -------------------------------------------------------
// Board
// -------------------------------------------------------
class Board {

enum BoardMode {
	BM_FILLING,
	BM_FLASHING,
	BM_MOVING,
	BM_READY
};

//typedef std::vector<ds::Sprite> Highlights;
//typedef std::vector<ds::GridPoint> Points;
//typedef std::vector<ds::DroppedCell> DroppedCells;
typedef std::vector<MovingCell> MovingCells;

public:
	Board(SpriteBuffer* buffer, RID textureID, GameSettings* settings);
	virtual ~Board();
	void fill(int maxColors);
	int select(const v2& mousePos);
	void move(const v2& mousePos);
	int getMovesLeft() {
		return 100;
	}
	void init();
	void update(float elasped);
	void render();
	void debug();
	void debugContainer();
	void rebuild();
private:
	void drawGrid();
	void drawFillGrid();

	ColorGrid m_Grid;
	//Points m_Points;
	//DroppedCells m_DroppedCells;
	MovingCells m_MovingCells;
	BoardMode m_Mode;
	float m_Timer;
	int m_CellCounter;
	v4 m_GridTex[3];
	int m_Counter;
	GameSettings* _settings;
	v2 _selectedPiece;
	int _flashCount;

	SpriteBuffer* _buffer;
	RID _textureID;
	
};

