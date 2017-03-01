#include "HieroFont.h"
#include <fstream>
#include <vector>

// char id=10    x=0    y=0    width=0    height=0    xoffset=0    yoffset=0    xadvance=0    page=0    chnl=0 
int HieroFont::extractInt(const char* txt) {
	// find = signs
	const char* c = txt;
	while (*c != '=') {
		++c;
	}
	++c;
	return atoi(c);
}

bool HieroFont::load(const char* fileName) {
	std::ifstream file(fileName);
	if (!file) {
		printf("file not found!\n");
		return false;
	}
	char cmd[256] = { 0 };
	int cnt = 0;
	int current = -1;
	for (;; ) {
		file >> cmd;
		if (!file) {
			break;
		}
		else if (strncmp(cmd, "id=",3) == 0) {
			current = extractInt(cmd);
			++cnt;
		}
		else if (strncmp(cmd, "width=", 6) == 0) {
			if (current != -1) {
				HieroFontItem& item = _items[current];
				item.width = extractInt(cmd);
			}
		}
		else if (strncmp(cmd, "height=", 6) == 0) {
			if (current != -1) {
				HieroFontItem& item = _items[current];
				item.height = extractInt(cmd);
			}
		}
		else if (strncmp(cmd, "x=", 2) == 0) {
			if (current != -1) {
				HieroFontItem& item = _items[current];
				item.x = extractInt(cmd);
			}
		}
		else if (strncmp(cmd, "y=", 2) == 0) {
			if (current != -1) {
				HieroFontItem& item = _items[current];
				item.y = extractInt(cmd);
			}
		}
		else if (strncmp(cmd, "xadvance=", 9) == 0) {
			if (current != -1) {
				HieroFontItem& item = _items[current];
				item.xadvance = extractInt(cmd);
			}
		}
		else if (strncmp(cmd, "yoffset=", 7) == 0) {
			if (current != -1) {
				HieroFontItem& item = _items[current];
				item.yoffset = extractInt(cmd);
			}
		}
		else if (strncmp(cmd, "xoffset=", 7) == 0) {
			if (current != -1) {
				HieroFontItem& item = _items[current];
				item.xoffset = extractInt(cmd);
			}
		}
	}
	// nasty workaround for space (char 32). Since Hiero exports it with 0 dimension
	//HieroFontItem& item = _items[32];
	//item.x = 0;
	//item.y = 96;
	//item.width = 12;
	//item.height = 32;
	return true;
}