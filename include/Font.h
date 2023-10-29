#ifndef ODIS_FONT_H
#define ODIS_FONT_H

#include <array>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "OdisMath.h"

const uint8_t NUM_FONT_CHARACTERS = 128;

struct Character
{
	unsigned int texture_id;
	ivec2 size;
	ivec2 bearing;
	unsigned int advance;
};

struct Font
{
	ivec2 size;
	std::string name;
	std::array<Character, NUM_FONT_CHARACTERS> characters;
};

#endif