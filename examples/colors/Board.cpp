#include "Board.h"
#include "Map.h"
#include "..\common\SpriteBuffer.h"
#include "GameSettings.h"
#include "utils\tweening.h"

Board::Board(SpriteBuffer* buffer, RID textureID, GameSettings* settings) : _buffer(buffer) , _textureID(textureID) , _settings(settings) {
	m_GridTex[0] = v4(420, 200, 430, 486);
	m_GridTex[1] = v4(450, 200, 320, 486);
	m_GridTex[2] = v4(860, 200, 110, 486);
}

Board::~Board(void) {}

// -------------------------------------------------------
// Init
// -------------------------------------------------------
void Board::init() {
}

// -------------------------------------------------------
// Fill board
// -------------------------------------------------------
void Board::fill(int maxColors) {
	m_CellCounter = 0;
	for ( int x = 0; x < MAX_X; ++x ) {		
		for ( int y = 0; y < MAX_Y; ++y ) {		
			int cid = ds::random(0, maxColors);
			int offset = offset = cid * CELL_SIZE;
			v2 p = grid::convertFromGrid(x, y);
			MyEntry& e = m_Grid.get(x, y);
			e.color = cid;
			e.hidden = false;
			e.texture = v4(offset, 680.0f, CELL_SIZE, CELL_SIZE);
			e.scale = 1.0f;
			e.state = TS_NORMAL;
			e.timer = 0.0f;
			m_Grid.set(x, y, e);
			++m_CellCounter;
		}
	}
	m_Mode = BM_FILLING;
	m_Timer = 0.0f;
	m_Counter = MAX_X * MAX_Y;
	_selectedPiece.x = -1;
	_selectedPiece.y = -1;
	_flashCount = 0;
}

// -------------------------------------------------------
// Draw
// -------------------------------------------------------
void Board::render() {
	_buffer->add(v2(295, 362), _textureID, m_GridTex[0]);
	_buffer->add(v2(670, 362), _textureID, m_GridTex[1]);
	_buffer->add(v2(885, 362), _textureID, m_GridTex[2]);
	// pieces
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!m_Grid.isFree(x, y)) {
				MyEntry& e = m_Grid.get(x, y);
				if (m_Mode == BM_FILLING) {
					float norm = m_Timer / _settings->tweeningTTL;
					v2 wp = grid::convertFromGrid(x, y);
					v2 sp = wp;
					sp.y += _settings->tweeningYAdd + y * _settings->tweeningYOffset;
					_buffer->add(tweening::interpolate(&tweening::linear, sp, wp, m_Timer,_settings->tweeningTTL),_textureID, e.texture);
				}
				else if (!e.hidden) {
					_buffer->add(grid::convertFromGrid(x, y),_textureID,e.texture,v2(e.scale));
				}
			}
		}
	}

	// moving cells
	//for (size_t i = 0; i < m_MovingCells.size(); ++i) {
		//sp.position = m_MovingCells[i].current;
		//sp.texture = m_MovingCells[i].texture;
		//ds::sprites::draw(sp);
	//}
}

// -------------------------------------------------------
// Update
// -------------------------------------------------------
void Board::update(float elapsed) {
	
	if (m_Mode == BM_FILLING) {
		m_Timer += elapsed;
		if (m_Timer > _settings->tweeningTTL) {
			m_Mode = BM_READY;
			m_Timer = 0.0f;
		}
	}
	/*
	else if (m_Mode == BM_FLASHING) {
		m_Timer += elapsed;
		if (m_Timer > m_Settings->flashTTL) {
			m_Mode = BM_READY;
			m_Timer = 0.0f;
			m_Grid.remove(m_Points);
			m_DroppedCells.clear();
			m_Grid.dropCells(m_DroppedCells);
			for (size_t i = 0; i < m_DroppedCells.size(); ++i) {
				ds::DroppedCell* dc = &m_DroppedCells[i];
				MyEntry& e = m_Grid.get(dc->to.x, dc->to.y);
				e.hidden = true;
				MovingCell m;
				m.x = dc->to.x;
				m.y = dc->to.y;
				m.texture = e.texture;
				m.start = grid::convertFromGrid(dc->from.x, dc->from.y);
				m.end = grid::convertFromGrid(dc->to.x, dc->to.y);
				m_MovingCells.push_back(m);
			}
			if (!m_DroppedCells.empty()) {
				m_Mode = BM_MOVING;
				m_Timer = 0.0f;
			}			
		}
		
	}
	else if (m_Mode == BM_MOVING) {
		m_Timer += elapsed;
		if (m_Timer > m_Settings->droppingTTL) {
			m_Mode = BM_READY;
			m_Timer = 0.0f;
			for (size_t i = 0; i < m_MovingCells.size(); ++i) {
				MovingCell& m = m_MovingCells[i];
				MyEntry& e = m_Grid.get(m.x, m.y);
				e.hidden = false;
			}
			m_MovingCells.clear();
		}
		else {
			float norm = m_Timer / m_Settings->droppingTTL;
			for (size_t i = 0; i < m_MovingCells.size(); ++i) {
				MovingCell& m = m_MovingCells[i];
				m.current = tweening::interpolate(&tweening::easeOutQuad, m.start, m.end, norm);
			}
		}
	}
	*/
	else if (m_Mode == BM_READY) {
		v2 mousePos = ds::getMousePosition();
		int my = ((int)mousePos.y - STARTY + HALF_CELL_SIZE)/CELL_SIZE;
		int mx = ((int)mousePos.x - STARTX + HALF_CELL_SIZE)/CELL_SIZE;

		if (mx >= 0 && my >= 0) {
			if (mx != _selectedPiece.x || my != _selectedPiece.y) {
				_selectedPiece = v2(mx,my);
				MyEntry& me = m_Grid(mx, my);
				if (me.state == TS_NORMAL) {
					me.timer = 0.0f;
					me.state = TS_WIGGLE;
				}
			}
		}
	}
	
	for (int x = 0; x < MAX_X; ++x) {
		for (int y = 0; y < MAX_Y; ++y) {
			if (!m_Grid.isFree(x, y)) {
				MyEntry& e = m_Grid.get(x, y);
				/*
				if (e.state == TS_SHRINKING) {
					e.timer += elapsed;
					if (e.timer >= m_Settings->flashTTL) {
						e.state = TS_NORMAL;
						e.scale = 1.0f;
						--_flashCount;
					}
					else {
						float norm = e.timer / m_Settings->flashTTL;
						e.scale = 1.0f - norm * 0.9f;
					}
				}
				else */
				if (e.state == TS_WIGGLE) {
					e.timer += elapsed;
					// FIXME: wiggleTTL
					if (e.timer >= 0.4f) {
						e.state = TS_NORMAL;
						e.scale = 1.0f;
					}
					else {
						float norm = e.timer / 0.4f;
						e.scale = 1.0f + sin(norm * ds::TWO_PI * 2.0f) * 0.1f;
					}
				}
			}
		}
	}
}

// -------------------------------------------------------
// move
// -------------------------------------------------------
void Board::move(const v2& mousePos) {
	v2 converted = grid::convertMousePosToGridPos(mousePos);
	if ( converted.x > 0 && converted.y > 0) {
		//LOG << "move grid pos " << converted.x << " " << converted.y;
		m_Grid.shiftColumns(converted.x);
	}
}

void Board::rebuild() {
	fill(4);
	m_MovingCells.clear();
	m_Mode = BM_FILLING;
}
// -------------------------------------------------------
// Select
// -------------------------------------------------------
int Board::select(const v2& mousePos) {
	/*
	int ret = -1;
	if ( m_Mode == BM_READY ) {
		v2 converted = grid::convertMousePosToGridPos(mousePos);
		if ( converted.x >= 0 && converted.y >= 0) {
			MyEntry& me = m_Grid(converted.x, converted.y);
			m_Points.clear();		
			m_Grid.findMatchingNeighbours(converted.x,converted.y,m_Points);
			if ( m_Points.size() > 1 ) {
				m_Timer = 0.0f;
				m_Mode = BM_FLASHING;
				ret = m_Points.size();				
				for ( size_t i = 0; i < m_Points.size(); ++i ) {
					ds::GridPoint* gp = &m_Points[i];
					MyEntry& c = m_Grid.get(gp->x, gp->y);
					c.state = TS_SHRINKING;
					c.timer = 0.0f;
					++_flashCount;
				}
				LOG << "flash count: " << _flashCount;
			}
		}
	}
	else {
		LOG << "Board is not ready";
	}
	return ret;
	*/
	return 0;
}

void Board::debugContainer() {
}
// -------------------------------------------------------
// debug
// -------------------------------------------------------
void Board::debug() {
	char buffer[32];
	//LOG << "---------------------- GRID ------------------------";
	for (int y = MAX_Y - 1; y >= 0; --y) {
		std::string line;
		for (int x = 0; x < MAX_X; ++x) {
			if (m_Grid.isFree(x, y)) {
				line += " -- ";
			}
			else {
				MyEntry& c = m_Grid.get(x, y);
				sprintf(buffer, " %2d ", c.color);
				line += buffer;
			}
		}
		//LOG << line;
	}
}