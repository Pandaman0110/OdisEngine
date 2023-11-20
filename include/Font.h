#ifndef ODIS_FONT_H
#define ODIS_FONT_H

#include <array>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "utility/OdisMath.h"

namespace OdisEngine
{
	const uint8_t NUM_FONT_CHARACTERS = 128;

	/**
	 * This struct represents a single glyph in a font
	 * 
	 */
	struct Character
	{
		/// the ID handle of the glyph texture
		uint32_t texture_id;

		/// size in pixels of the glyph
		glm::ivec2 size;

		/// horizontal offset from the glyph origin in pixels, and vertical offset from the baseline
		glm::ivec2 bearing;

		///the horizontal distance in 1/64th pixels to the next glyph in the font
		uint32_t advance;
	};

	struct Font
	{
		glm::ivec2 size;
		std::string name;
		std::array<Character, NUM_FONT_CHARACTERS> characters;
	};
}

#endif