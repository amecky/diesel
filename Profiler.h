#pragma once
#include <Windows.h>
#include <stdint.h>
#include <vector>
namespace perf {

	void init();

	void reset();

	void finalize();

	int start(const char* name);

	void end(int index);

	void shutdown();

	void debug();

	//void save(const ds::ReportWriter& writer);

	void tickFPS(float dt);

	void incFrame();

	float get_current_total_time();

	class ZoneTracker {

	public:
		ZoneTracker(const char* name) {
			_index = perf::start(name);
		}
		~ZoneTracker() {
			perf::end(_index);
		}
	private:
		int _index;
	};
}

namespace perf {

	struct CharBuffer {

		char* data;
		int size;
		int capacity;
		int num;

		CharBuffer();
		~CharBuffer();

		void* alloc(int sz);
		void resize(int newCap);
		int append(const char* s, int len);
		int append(const char* s);
		int append(char s);
		const char* get(int index) const;

	};

	const uint32_t FNV_Prime = 0x01000193; //   16777619
	const uint32_t FNV_Seed = 0x811C9DC5; // 2166136261

	inline uint32_t fnv1a(const char* text, uint32_t hash = FNV_Seed) {
		const unsigned char* ptr = (const unsigned char*)text;
		while (*ptr) {
			hash = (*ptr++ ^ hash) * FNV_Prime;
		}
		return hash;
	}

	uint32_t murmur_hash(const char* text);
	uint32_t murmur_hash(const void * key, int len, uint32_t seed);

#define SID(str) (StaticHash(str))
#define SID_VAL(str) (fnv1a(str))

	// ----------------------------------------------------
	// Static Hash 
	// ----------------------------------------------------
	class StaticHash {

	public:
		StaticHash() {
			_hash = 0;
		}
		explicit StaticHash(const char* text) {
			_hash = fnv1a(text);
		}
		explicit StaticHash(uint32_t hash) {
			_hash = hash;
		}
		~StaticHash() {}
		const uint32_t get() const {
			return _hash;
		}
		const bool operator<(const StaticHash &rhs) const {
			return _hash < rhs.get();
		}
	private:
		uint32_t _hash;
	};

	const StaticHash INVALID_HASH = StaticHash(static_cast<unsigned int>(0));

	const bool operator==(const StaticHash& lhs, const StaticHash &rhs);

	const bool operator!=(const StaticHash& lhs, const StaticHash &rhs);

	// -----------------------------------------------------------
	// zone tracker event
	// -----------------------------------------------------------
	struct ZoneTrackerEvent {
		StaticHash hash;
		int parent;
		LARGE_INTEGER started;
		float duration;
		int name_index;
		int ident;
	};

	// -----------------------------------------------------------
	// Zone tracker context
	// -----------------------------------------------------------
	struct ZoneTrackerContext {
		LARGE_INTEGER frequency;
		CharBuffer names;
		std::vector<StaticHash> hashes;
		int current_parent;
		int root_event;
		std::vector<ZoneTrackerEvent> events;
		int ident;
		int frames;
		float fpsTimer;
		int fps;
		LONGLONG overhead;
	};

	static ZoneTrackerContext* zoneTrackerCtx = 0;

	double LIToSecs(LARGE_INTEGER & L) {
		return (L.QuadPart - zoneTrackerCtx->overhead) * 1000.0 / zoneTrackerCtx->frequency.QuadPart;
	}

	// -----------------------------------------------------------
	// init
	// -----------------------------------------------------------
	void init() {
		if (zoneTrackerCtx == 0) {
			zoneTrackerCtx = new ZoneTrackerContext;
			QueryPerformanceFrequency(&zoneTrackerCtx->frequency);
			LARGE_INTEGER start;
			LARGE_INTEGER stop;
			QueryPerformanceCounter(&start);
			QueryPerformanceCounter(&stop);
			zoneTrackerCtx->overhead = stop.QuadPart - start.QuadPart;
		}
		zoneTrackerCtx->frames = 0;
		zoneTrackerCtx->fpsTimer = 0.0f;
		zoneTrackerCtx->fps = 0;
	}

	// -----------------------------------------------------------
	// reset
	// -----------------------------------------------------------
	void reset() {
		zoneTrackerCtx->names.size = 0;
		zoneTrackerCtx->events.clear();
		zoneTrackerCtx->ident = 0;
		// create root event
		zoneTrackerCtx->current_parent = -1;
		zoneTrackerCtx->root_event = start("ROOT");
		zoneTrackerCtx->current_parent = zoneTrackerCtx->root_event;
	}

	// -----------------------------------------------------------
	// finalize
	// -----------------------------------------------------------
	void finalize() {
		end(zoneTrackerCtx->root_event);
	}

	// -----------------------------------------------------------
	// debug
	// -----------------------------------------------------------
	void debug() {
		/*
		LOG << "------------------------------------------------------------";
		LOG << " Percent | Accu       | Name";
		LOG << "------------------------------------------------------------";
		float norm = zoneTrackerCtx->events[0].duration;
		char buffer[256];
		char p[10];
		std::string line;
		for (uint32_t i = 0; i < zoneTrackerCtx->events.size(); ++i) {
			const ZoneTrackerEvent& event = zoneTrackerCtx->events[i];
			int ident = event.ident * 2;
			float per = event.duration / norm * 100.0f;
			ds::string::formatPercentage(per, p);
			sprintf(buffer, "%s  | %3.8f | ", p, event.duration);
			line = buffer;
			for (int j = 0; j < ident; ++j) {
				line += " ";
			}
			const char* n = zoneTrackerCtx->names.data + zoneTrackerCtx->events[i].name_index;
			LOG << line << " " << n;
		}
		LOG << "------------------------------------------------------------";
		*/
	}

	// -----------------------------------------------------------
	// tick FPS
	// -----------------------------------------------------------
	void tickFPS(float dt) {
		zoneTrackerCtx->fpsTimer += dt;
		if (zoneTrackerCtx->fpsTimer >= 1.0f) {
			zoneTrackerCtx->fpsTimer -= 1.0f;
			zoneTrackerCtx->fps = zoneTrackerCtx->frames;
			zoneTrackerCtx->frames = 0;
		}
	}

	// -----------------------------------------------------------
	// increment frame
	// -----------------------------------------------------------
	void incFrame() {
		++zoneTrackerCtx->frames;
	}

	// -----------------------------------------------------------
	// find by hash
	// -----------------------------------------------------------
	int findHash(const StaticHash& hash) {
		for (uint32_t i = 0; i < zoneTrackerCtx->events.size(); ++i) {
			if (zoneTrackerCtx->events[i].hash == hash) {
				return i;
			}
		}
		return -1;
	}

	// -----------------------------------------------------------
	// start zone tracking
	// -----------------------------------------------------------
	int start(const char* name) {
		// create event
		ZoneTrackerEvent event;
		event.parent = zoneTrackerCtx->current_parent;
		QueryPerformanceCounter(&event.started);
		event.ident = zoneTrackerCtx->ident++;
		event.hash = StaticHash(name);
		event.duration = -1.0f;
		int idx = findHash(event.hash);
		if (idx == -1) {
			event.name_index = zoneTrackerCtx->names.size;
			int l = strlen(name);
			if (zoneTrackerCtx->names.size + l > zoneTrackerCtx->names.capacity) {
				zoneTrackerCtx->names.resize(zoneTrackerCtx->names.capacity + 256);
			}
			zoneTrackerCtx->names.append(name, l);
		}
		else {
			event.name_index = zoneTrackerCtx->events[idx].name_index;
		}
		int eventIndex = zoneTrackerCtx->events.size();
		zoneTrackerCtx->events.push_back(event);
		zoneTrackerCtx->current_parent = eventIndex;
		return eventIndex;
	}

	// -----------------------------------------------------------
	// end zone tracking
	// -----------------------------------------------------------
	void end(int index) {

		ZoneTrackerEvent& event = zoneTrackerCtx->events[index];
		LARGE_INTEGER EndingTime;
		QueryPerformanceCounter(&EndingTime);
		LARGE_INTEGER time;
		time.QuadPart = EndingTime.QuadPart - event.started.QuadPart;
		event.duration = LIToSecs(time);
		if (zoneTrackerCtx->events[zoneTrackerCtx->current_parent].parent != -1) {
			zoneTrackerCtx->current_parent = zoneTrackerCtx->events[zoneTrackerCtx->current_parent].parent;
		}
		--zoneTrackerCtx->ident;
	}

	// -----------------------------------------------------------
	// shutdown
	// -----------------------------------------------------------
	void shutdown() {
		delete zoneTrackerCtx;
	}

	CharBuffer::CharBuffer() : data(nullptr), size(0), capacity(0), num(0) {}

	CharBuffer& CharBuffer::operator =(const CharBuffer& b) {
		int sz = b.size;
		resize(sz);
		memcpy(data, b.data, sz);
		return *this;
	}

	CharBuffer::~CharBuffer() {
		if (data != nullptr) {
			delete[] data;
		}
	}

	void* CharBuffer::alloc(int sz) {
		if (size + sz > capacity) {
			int d = capacity * 2 + 8;
			if (d < sz) {
				d = sz * 2 + 8;
			}
			resize(d);
		}
		auto res = data + size;
		size += sz;
		int d = sz / 4;
		if (d == 0) {
			d = 1;
		}
		num += d;
		return res;
	}

	void CharBuffer::resize(int newCap) {
		if (newCap > capacity) {
			char* tmp = new char[newCap];
			if (data != nullptr) {
				memcpy(tmp, data, size);
				delete[] data;
			}
			capacity = newCap;
			data = tmp;
		}
	}

	const char* CharBuffer::get(int index) const {
		return data + index;
	}

	int CharBuffer::append(const char* s, int len) {
		if (size + len + 1 > capacity) {
			resize(capacity + len + 1 + 8);
		}
		const char* t = s;
		int ret = size;
		for (int i = 0; i < len; ++i) {
			data[size++] = *t;
			++t;
		}
		data[size++] = '\0';
		return ret;
	}

	int CharBuffer::append(const char* s) {
		int len = strlen(s);
		return append(s, len);
	}

	int CharBuffer::append(char s) {
		if (size + 1 > capacity) {
			resize(capacity + 9);
		}
		int ret = size;
		data[size++] = s;
		data[size++] = '\0';
		return ret;
	}
}
