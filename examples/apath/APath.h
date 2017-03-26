#pragma once

struct p2i {
	int x;
	int y;

	p2i() : x(0), y(0) {}
	p2i(int xp, int yp) : x(xp), y(yp) {}
};

inline bool operator ==(const p2i& first, const p2i& other) {
	return first.x == other.x && first.y == other.y;
}

inline bool operator !=(const p2i& first, const p2i& other) {
	return !(first == other);
}

const p2i INVALID_POINT = p2i(-1, -1);

struct Grid {

	int* items;
	int width;
	int height;
	p2i start;
	p2i end;

	Grid() : items(0), width(0), height(0) , start(-1,-1) , end(-1,-1) {}

	Grid(int w, int h) : width(w) , height(h) {
		int total = width * height;
		items = new int[total];
		for (int i = 0; i < total; ++i) {
			items[i] = 0;
		}
	}

	~Grid() {
		if (items != 0) {
			delete[] items;
		}
	}

	void setStart(int x, int y) {
		setStart(p2i(x, y));
	}

	void setStart(p2i s) {
		if (isValid(s) && isAvailable(s)) {
			if (s != INVALID_POINT) {
				set(start, 0);
			}
			set(s, 2);
			start = s;
		}
	}

	void setEnd(int x, int y) {
		setEnd(p2i(x, y));
	}

	void setEnd(p2i e) {
		if (isValid(e) && isAvailable(e)) {
			if (end != INVALID_POINT) {
				set(end, 0);
			}
			set(e, 3);
			end = e;
		}
	}

	bool isValid(p2i p) const {
		return isValid(p.x, p.y);
	}

	void set(p2i p, int v) {
		set(p.x, p.y, v);
	}

	void set(int x, int y, int v) {
		if (isValid(x, y)) {
			int idx = x + y * width;
			items[idx] = v;
		}
	}

	bool isValid(int x, int y) const {
		if (x < 0 || y < 0 || x >= width || y >= height) {
			return false;
		}
		return true;
	}

	bool isAvailable(p2i p) const {
		return isAvailable(p.x, p.y);
	}

	bool isAvailable(int x, int y) const {
		if (isValid(x, y)) {
			int idx = x + y * width;
			return items[idx] != 1;
		}
		return false;
	}

	int getIndex(p2i p) const {
		return getIndex(p.x, p.y);
	}

	int getIndex(int x, int y) const {
		return x + y * width;
	}
};





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