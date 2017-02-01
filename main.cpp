#define DS_IMPLEMENTATION
#include "diesel.h"

int main(const char** args) {
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	if (ds::init(rs)) {
		while (ds::isRunning()) {
			ds::begin();
			ds::end();
		}
		ds::shutdown();
	}
}