#pragma once
#include "..\..\diesel.h"

const float MOVE_TTL = 1.2f;
const int CELL_SIZE = 40;
const int HALF_CELL_SIZE = 20;
const int STARTX = 130;
const int STARTY = 144;
const int MAX_X = 20;
const int MAX_Y = 12;

const float MESSAGE_TTL = 0.8f;
const float MESSAGE_SCALE = 0.4f;

// font texture rects starting with character 32
const ds::vec4 FONT_RECTS[] = {
	ds::vec4(250, 200, 14, 16),
	ds::vec4(220, 254, 6, 16),
	ds::vec4(152, 270, 11, 9),
	ds::vec4(39, 270, 14, 16),
	ds::vec4(0, 270, 16, 16),
	ds::vec4(16, 270, 23, 16),
	ds::vec4(53, 270, 20, 16),
	ds::vec4(171, 270, 5, 8),
	ds::vec4(0, 200, 9, 22),
	ds::vec4( 9, 200, 9, 22),
	ds::vec4(125, 270, 14, 13),
	ds::vec4(112, 270, 13, 13),
	ds::vec4(163, 270, 8, 9),
	ds::vec4(204, 270, 7, 5),
	ds::vec4(185, 270, 6, 6),
	ds::vec4(53, 200, 15, 19),
	ds::vec4(201, 254, 18, 16),
	ds::vec4(55, 254, 8, 16),
	ds::vec4(64, 254, 16, 16),
	ds::vec4(81, 254, 15, 16),
	ds::vec4(97, 254, 17, 16),
	ds::vec4(115, 254, 17, 16),
	ds::vec4(133, 254, 16, 16),
	ds::vec4(150, 254, 16, 16),
	ds::vec4(167, 254, 16, 16),
	ds::vec4(184, 254, 16, 16), 
	ds::vec4(247, 254, 5, 13), // :
	ds::vec4(240, 254, 6, 16),
	ds::vec4(73, 270, 12, 14),
	ds::vec4(139, 270, 13, 10),
	ds::vec4(86, 270, 13, 14),
	ds::vec4(226, 254, 14, 16), 
	ds::vec4(68, 200, 17, 19),
	ds::vec4(141, 200, 20, 16), // A
	ds::vec4(162, 200, 16, 16), 
	ds::vec4(179, 200, 16, 16), 
	ds::vec4(196, 200, 17, 16),
	ds::vec4(214, 200, 15, 16),
	ds::vec4(230, 200, 15, 16),
	ds::vec4(0, 222, 16, 16),
	ds::vec4(17, 222, 17, 16),
	ds::vec4(246, 200, 8, 16),
	ds::vec4(35, 222, 14, 16),
	ds::vec4(50, 222, 18, 16),
	ds::vec4(69, 222, 15, 16),
	ds::vec4(85, 222, 22, 16),
	ds::vec4(108, 222, 18, 16),
	ds::vec4(127, 222, 18, 16),
	ds::vec4(146, 222, 17, 16),
	ds::vec4(100, 200, 19, 18),
	ds::vec4(164, 222, 17, 16),
	ds::vec4(182, 222, 16, 16),
	ds::vec4(199, 222, 16, 16),
	ds::vec4(217, 222, 17, 16),
	ds::vec4(0, 238, 20, 16),
	ds::vec4(21, 238, 26, 16),
	ds::vec4(48, 238, 21, 16),
	ds::vec4(235, 222, 19, 16),
	ds::vec4(70, 238, 16, 16),
	ds::vec4(18, 200, 7, 22),
	ds::vec4(85, 200, 15, 19),
	ds::vec4(25, 200, 8, 22),
	ds::vec4(99, 270, 13, 14),
	ds::vec4(211, 270, 17, 5),
	ds::vec4(176, 270, 9, 7),
	ds::vec4(141, 200, 20, 16), // a
	ds::vec4(162, 200, 16, 16),
	ds::vec4(179, 200, 16, 16),
	ds::vec4(196, 200, 17, 16),
	ds::vec4(214, 200, 15, 16),
	ds::vec4(230, 200, 15, 16),
	ds::vec4(0, 222, 16, 16),
	ds::vec4(17, 222, 17, 16),
	ds::vec4(246, 200, 9, 16),
	ds::vec4(35, 222, 14, 16),
	ds::vec4(50, 222, 18, 16),
	ds::vec4(69, 222, 15, 16),
	ds::vec4(85, 222, 22, 16),
	ds::vec4(108, 222, 18, 16),
	ds::vec4(127, 222, 18, 16),
	ds::vec4(146, 222, 17, 16),
	ds::vec4(100, 200, 19, 18),
	ds::vec4(164, 222, 17, 16),
	ds::vec4(182, 222, 16, 16),
	ds::vec4(199, 222, 17, 16),
	ds::vec4(217, 222, 17, 16),
	ds::vec4(0, 238, 20, 16),
	ds::vec4(21, 238, 26, 16),
	ds::vec4(48, 238, 21, 16),
	ds::vec4(235, 222, 19, 16),
	ds::vec4(70, 238, 16, 16),
	ds::vec4(18, 200, 7, 22),
	ds::vec4(48, 200, 5, 22),
	ds::vec4(40, 200, 8, 22),
	ds::vec4(191, 270, 13, 6)
};
