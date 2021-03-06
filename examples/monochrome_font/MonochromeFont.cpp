#define DS_IMPLEMENTATION
#include "..\..\diesel.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\common\stb_image.h"
#define SPRITE_IMPLEMENTATION
#include "..\common\SpriteBatchBuffer.h"

const unsigned char font[128][16] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x60, 0x60, 0x06, 0x1B, 0x3C, 0x6C, 0xC0, 0x01, 0x06, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0xF0, 0x60, 0x06, 0x1B, 0x66, 0x6C, 0x61, 0x03, 0x06, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0xF0, 0x60, 0x86, 0x3F, 0x06, 0xB8, 0x61, 0x03, 0x06, 0x18, 0x60, 0xC0, 0x06, 0x06, 0x00 },
	{ 0x00, 0xF0, 0x00, 0x00, 0x1B, 0x0C, 0xC0, 0xC0, 0x01, 0x00, 0x0C, 0xC0, 0x80, 0x03, 0x06, 0x00 },
	{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x18, 0x60, 0x60, 0x00, 0x00, 0x0C, 0xC0, 0xE0, 0x8F, 0x1F, 0x00 },
	{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x30, 0x30, 0x60, 0x0F, 0x00, 0x0C, 0xC0, 0x80, 0x03, 0x06, 0x00 },
	{ 0x00, 0x00, 0x00, 0x80, 0x3F, 0x60, 0xD8, 0x61, 0x06, 0x00, 0x0C, 0xC0, 0xC0, 0x06, 0x06, 0x00 },
	{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x66, 0x68, 0x63, 0x06, 0x00, 0x0C, 0xC0, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x60, 0x00, 0x00, 0x1B, 0x3C, 0x60, 0xC3, 0x0D, 0x00, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xC0, 0x01, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x18, 0x78, 0xC0, 0xC0, 0x03, 0x0F, 0x0C, 0xF8, 0x81, 0x83, 0x1F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x18, 0xCC, 0xE0, 0x60, 0x86, 0x19, 0x0C, 0x18, 0x80, 0x01, 0x18, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0xEC, 0xF8, 0x60, 0x86, 0x19, 0x6C, 0x18, 0xC0, 0x00, 0x0C, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0xEC, 0xC0, 0x00, 0x06, 0x18, 0x6C, 0x18, 0xE0, 0x03, 0x0C, 0x00 },
	{ 0x00, 0xF8, 0x01, 0x00, 0x06, 0xCC, 0xC0, 0x00, 0x03, 0x0E, 0x6C, 0xF8, 0x60, 0x06, 0x06, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x06, 0xDC, 0xC0, 0x80, 0x01, 0x18, 0x66, 0x80, 0x61, 0x06, 0x06, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x03, 0xDC, 0xC0, 0xC0, 0x80, 0x19, 0xFE, 0x80, 0x61, 0x06, 0x03, 0x00 },
	{ 0x38, 0x00, 0x80, 0x03, 0x03, 0xCC, 0xC0, 0x60, 0x80, 0x19, 0x60, 0xC0, 0x60, 0x06, 0x03, 0x00 },
	{ 0x38, 0x00, 0x80, 0x83, 0x01, 0x78, 0xC0, 0xE0, 0x07, 0x0F, 0x60, 0x78, 0xC0, 0x03, 0x03, 0x00 },
	{ 0x30, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3C, 0xF0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x0F, 0x7E, 0x60, 0xE0, 0x03, 0x0F, 0x00 },
	{ 0x66, 0x98, 0x01, 0x00, 0x00, 0x30, 0x00, 0xC0, 0x80, 0x19, 0xC3, 0xF0, 0x60, 0x86, 0x19, 0x00 },
	{ 0x66, 0x98, 0x81, 0x03, 0x0E, 0x18, 0x00, 0x80, 0x81, 0x19, 0xC3, 0x98, 0x61, 0x86, 0x19, 0x00 },
	{ 0x6E, 0x98, 0x81, 0x03, 0x0E, 0x0C, 0xF8, 0x01, 0x03, 0x0C, 0xF3, 0x98, 0x61, 0x86, 0x01, 0x00 },
	{ 0x3C, 0x98, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x06, 0x06, 0xDB, 0x98, 0xE1, 0x83, 0x01, 0x00 },
	{ 0x76, 0xF0, 0x01, 0x00, 0x00, 0x0C, 0xF8, 0x01, 0x03, 0x06, 0xDB, 0xF8, 0x61, 0x86, 0x01, 0x00 },
	{ 0x66, 0xC0, 0x00, 0x00, 0x00, 0x18, 0x00, 0x80, 0x01, 0x00, 0xF3, 0x98, 0x61, 0x86, 0x19, 0x00 },
	{ 0x66, 0x60, 0x80, 0x03, 0x0E, 0x30, 0x00, 0xC0, 0x00, 0x06, 0x03, 0x98, 0x61, 0x86, 0x19, 0x00 },
	{ 0x3C, 0x70, 0x80, 0x03, 0x0E, 0x60, 0x00, 0x60, 0x00, 0x06, 0xFE, 0x98, 0xE1, 0x03, 0x0F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x1E, 0xF8, 0xE1, 0x07, 0x0F, 0x66, 0xF0, 0x00, 0x86, 0x19, 0x06, 0x18, 0x63, 0x0C, 0x0F, 0x00 },
	{ 0x36, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x00, 0x86, 0x19, 0x06, 0x18, 0x63, 0x8C, 0x19, 0x00 },
	{ 0x66, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x00, 0x86, 0x0D, 0x06, 0xB8, 0xE3, 0x8C, 0x19, 0x00 },
	{ 0x66, 0x18, 0x60, 0x80, 0x01, 0x66, 0x60, 0x00, 0x86, 0x0D, 0x06, 0x58, 0xE3, 0x8D, 0x19, 0x00 },
	{ 0x66, 0xF8, 0xE0, 0x83, 0x01, 0x7E, 0x60, 0x00, 0x86, 0x07, 0x06, 0x58, 0x63, 0x8F, 0x19, 0x00 },
	{ 0x66, 0x18, 0x60, 0x80, 0x1D, 0x66, 0x60, 0x00, 0x86, 0x0D, 0x06, 0x58, 0x63, 0x8E, 0x19, 0x00 },
	{ 0x66, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x60, 0x86, 0x0D, 0x06, 0x18, 0x63, 0x8C, 0x19, 0x00 },
	{ 0x36, 0x18, 0x60, 0x80, 0x19, 0x66, 0x60, 0x60, 0x86, 0x19, 0x06, 0x18, 0x63, 0x8C, 0x19, 0x00 },
	{ 0x1E, 0xF8, 0x61, 0x00, 0x1F, 0x66, 0xF0, 0xC0, 0x83, 0x19, 0x7E, 0x18, 0x63, 0x0C, 0x0F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3E, 0xF0, 0xE0, 0x03, 0x0F, 0x7E, 0x98, 0x61, 0x86, 0x31, 0x66, 0x98, 0xE1, 0x07, 0x0F, 0x00 },
	{ 0x66, 0x98, 0x61, 0x86, 0x19, 0x18, 0x98, 0x61, 0x86, 0x31, 0x66, 0x98, 0x01, 0x06, 0x03, 0x00 },
	{ 0x66, 0x98, 0x61, 0x86, 0x01, 0x18, 0x98, 0x61, 0x86, 0x31, 0x2C, 0x98, 0x01, 0x06, 0x03, 0x00 },
	{ 0x66, 0x98, 0x61, 0x06, 0x03, 0x18, 0x98, 0x61, 0x86, 0x35, 0x18, 0x98, 0x01, 0x03, 0x03, 0x00 },
	{ 0x3E, 0x98, 0xE1, 0x03, 0x06, 0x18, 0x98, 0x61, 0x86, 0x35, 0x18, 0xF0, 0x80, 0x01, 0x03, 0x00 },
	{ 0x06, 0x98, 0x61, 0x03, 0x0C, 0x18, 0x98, 0x61, 0x86, 0x35, 0x34, 0x60, 0xC0, 0x00, 0x03, 0x00 },
	{ 0x06, 0x98, 0x61, 0x06, 0x18, 0x18, 0x98, 0x61, 0x06, 0x1B, 0x66, 0x60, 0x60, 0x00, 0x03, 0x00 },
	{ 0x06, 0x98, 0x61, 0x86, 0x19, 0x18, 0x98, 0xC1, 0x03, 0x1B, 0x66, 0x60, 0x60, 0x00, 0x03, 0x00 },
	{ 0x06, 0xF0, 0x60, 0x06, 0x0F, 0x18, 0xF0, 0x80, 0x01, 0x1B, 0x66, 0x60, 0xE0, 0x07, 0x03, 0x00 },
	{ 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00 },
	{ 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x80, 0x01, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xC0, 0x03, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x06, 0xF0, 0x60, 0x06, 0x00, 0x30, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0x80, 0x07, 0x00, 0x00 },
	{ 0x06, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x60, 0x00, 0xC0, 0x00, 0x00, 0x00 },
	{ 0x0C, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE0, 0x03, 0x0F, 0x7C, 0xF0, 0xC0, 0x00, 0x1F, 0x00 },
	{ 0x0C, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x61, 0x86, 0x19, 0x66, 0x98, 0xC1, 0x80, 0x19, 0x00 },
	{ 0x18, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x61, 0x86, 0x01, 0x66, 0x98, 0xE1, 0x87, 0x19, 0x00 },
	{ 0x18, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x61, 0x86, 0x01, 0x66, 0xF8, 0xC1, 0x80, 0x19, 0x00 },
	{ 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x98, 0x61, 0x86, 0x01, 0x66, 0x18, 0xC0, 0x80, 0x19, 0x00 },
	{ 0x30, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x98, 0x61, 0x86, 0x19, 0x66, 0x18, 0xC0, 0x80, 0x19, 0x00 },
	{ 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xE1, 0x03, 0x0F, 0x7C, 0xF0, 0xC0, 0x00, 0x1F, 0x00 },
	{ 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00 },
	{ 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00 },
	{ 0x00, 0xF0, 0x00, 0xC0, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x06, 0x60, 0x00, 0x83, 0x01, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x06, 0x00, 0x00, 0x80, 0x01, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x3E, 0x78, 0xC0, 0x83, 0x19, 0x18, 0xF8, 0xE1, 0x03, 0x0F, 0x3E, 0xF0, 0x61, 0x06, 0x1F, 0x00 },
	{ 0x66, 0x60, 0x00, 0x83, 0x19, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x87, 0x01, 0x00 },
	{ 0x66, 0x60, 0x00, 0x83, 0x0D, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0xE1, 0x80, 0x01, 0x00 },
	{ 0x66, 0x60, 0x00, 0x83, 0x07, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x00, 0x0F, 0x00 },
	{ 0x66, 0x60, 0x00, 0x83, 0x0D, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x00, 0x18, 0x00 },
	{ 0x66, 0x60, 0x00, 0x83, 0x19, 0x18, 0x58, 0x63, 0x86, 0x19, 0x66, 0x98, 0x61, 0x00, 0x18, 0x00 },
	{ 0x66, 0xF8, 0x01, 0x83, 0x19, 0x7E, 0x18, 0x63, 0x06, 0x0F, 0x3E, 0xF0, 0x61, 0x80, 0x0F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x01, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x01, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x01, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x18, 0x30, 0xE0, 0x88, 0x1F, 0x00 },
	{ 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x18, 0x60, 0xB0, 0x8D, 0x1F, 0x00 },
	{ 0x7E, 0x98, 0x61, 0x86, 0x31, 0x66, 0x98, 0xE1, 0x07, 0x06, 0x18, 0x60, 0x10, 0x87, 0x1F, 0x00 },
	{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x66, 0x98, 0x01, 0x06, 0x06, 0x18, 0x60, 0x00, 0x80, 0x1F, 0x00 },
	{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x3C, 0x98, 0x01, 0x03, 0x03, 0x18, 0xC0, 0x00, 0x80, 0x1F, 0x00 },
	{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x18, 0x98, 0x81, 0x81, 0x01, 0x18, 0x80, 0x01, 0x80, 0x1F, 0x00 },
	{ 0x0C, 0x98, 0x61, 0x86, 0x35, 0x3C, 0x98, 0xC1, 0x00, 0x03, 0x18, 0xC0, 0x00, 0x80, 0x1F, 0x00 },
	{ 0x0C, 0x98, 0xC1, 0x03, 0x1B, 0x66, 0x98, 0x61, 0x00, 0x06, 0x18, 0x60, 0x00, 0x80, 0x1F, 0x00 },
	{ 0x78, 0xF0, 0x81, 0x01, 0x1B, 0x66, 0xF0, 0xE0, 0x07, 0x06, 0x18, 0x60, 0x00, 0x80, 0x1F, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x06, 0x18, 0x60, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
};


void print(uint8_t t) {
	for (int i = 0; i < 8; ++i) {
		if ((t >> i) & 1) {
			printf("1 ");
		}
		else {
			printf("0 ");
		}
	}
	printf("\n");
}
uint8_t convert(unsigned char* data, int x, int y) {
	uint8_t ret = 0;
	for (int i = 0; i < 8; ++i) {
		int p = (x + i + y * 128) * 4;
		uint8_t r = data[p];
		if (r > 0) {
			ret |= 1 << i;
		}
	}
	return ret;
}

void convertImage() {
	// load image using stb_image
	int sx, sy, n;
	unsigned char *data = stbi_load("fixedsys.png", &sx, &sy, &n, 4);
		RID textureID = ds::createTexture(ds::TextureDesc()
		.Width(sx)
		.Height(sy)
		.Channels(n)
		.Data(data)
		.Format(ds::TextureFormat::R8G8B8A8_UNORM)
		.BindFlags(ds::BindFlag::BF_SHADER_RESOURCE)
	);
	int dx = sx / 8;
	FILE* fp = fopen("fixedsys.h","w");
	if (fp) {
		fprintf(fp, "const unsigned char font[%d][%d] = {\n", sy, dx);
		for (int y = 0; y < sy; ++y) {
			fprintf(fp, "\t{ ");
			for (int i = 0; i < dx; ++i) {
				uint8_t r = convert(data, i * 8, y);
				fprintf(fp, "0x%02X", r);
				if (i < 15) {
					fprintf(fp, ", ");
				}
			}
			fprintf(fp, " },\n");
		}
		fprintf(fp, "};\n");
		fclose(fp);
	}
	stbi_image_free(data);
}

void log(const LogLevel& lvl, const char* message) {
	OutputDebugString(message);
	OutputDebugString("\n");
}
// ---------------------------------------------------------------
// main method
// ---------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
//int main(int argc, char *argv[]) {
	//	
	// prepare application
	//
	ds::RenderSettings rs;
	rs.width = 1024;
	rs.height = 768;
	rs.title = "Hello world";
	rs.clearColor = ds::Color(0.2f, 0.2f, 0.2f, 1.0f);
	rs.multisampling = 4;
	rs.useGPUProfiling = false;
	rs.logHandler = &log;
	ds::init(rs);

	//convertImage();
	
	uint8_t* data = new uint8_t[128 * 128 * 4];
	int index = 0;
	for (int y = 0; y < 128; ++y) {
		for (int x = 0; x < 16; ++x) {
			uint8_t t = font[y][x];			
			for (int z = 0; z < 8; ++z) {
				if ((t >> z) & 1) {
					data[index++] = 255;
					data[index++] = 255;
					data[index++] = 255;
					data[index++] = 255;
				}
				else {
					data[index++] = 0;
					data[index++] = 0;
					data[index++] = 0;
					data[index++] = 0;
				}
			}
		}
	}
		
	RID textureID = ds::createTexture(ds::TextureDesc()
		.Width(128)
		.Height(128)
		.Channels(4)
		.Data(data)
		.Format(ds::TextureFormat::R8G8B8A8_UNORM)
		.BindFlags(ds::BindFlag::BF_SHADER_RESOURCE));
	SpriteBatchBufferInfo sbbInfo = { 1024,textureID };
	SpriteBatchBuffer buffer(sbbInfo);

	ds::logResources();

	const char* message = "-9E Hello World! This is 123.45 Test. Just get me going.";

	while (ds::isRunning()) {
	
		ds::begin();
	
		buffer.begin();

		int l = strlen(message);
		for (int i = 0; i < l; ++i) {
			int c = message[i] - 32;
			int t = c / 12;
			int y = t * 16;
			int x = (c - t * 12) * 10;
			buffer.add(ds::vec2(200 + 10 * i, 400), ds::vec4(x, y, 8, 14));
		}
		buffer.flush();
		
		ds::end();
	}
	ds::shutdown();
	return 0;
}