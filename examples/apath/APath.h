#pragma once
#include "Grid.h"

struct WayPoint {
	p2i p;
	float f;
	float h;
	float g;
	int parent;
	bool available;
	int index;
	WayPoint() : p(-1, -1), f(0.0f), h(0.0f), g(0.0f), parent(-1), available(true) {}
};

class APath {

public:
	APath(Grid* grid);
	~APath();
	int find(p2i start, p2i end, p2i* points, int max);
	int num() const {
		return _numClosed;
	}
	p2i get(int index) const {
		int idx = _closed[index];
		return _internalGrid[idx].p;
	}
	void print();
private:
	void print(const WayPoint& wp);
	void print(int index);
	int getIndex(p2i p);
	int moveNext(p2i p);
	bool visited(int idx);
	void close(p2i p);
	int find(p2i p);
	bool isOpen(int index);
	void removeWaypoint(p2i p);
	int findCheapest();
	int getNeighbours(p2i p, int* result, int max);
	float add(p2i p, float g = 0.0f, int parent = -1);
	float calculateG(p2i first, p2i second);
	float calculateG(int firstIndex, int secondIndex);
	float calculateH(p2i p);
	Grid* _grid;
	WayPoint* _internalGrid;
	int _width;
	int _height;
	int* _open;
	int* _closed;
	int _numOpen;
	int _numClosed;
	p2i _start;
	p2i _end;
};