#pragma once
#include <glm/glm.hpp>

namespace leo
{
	using Color = glm::u8vec4;

	// Some Basic Colors
	inline const Color WHITE{ 255, 255, 255, 255 };
	inline const Color BLACK{ 0,   0,   0, 255 };
	inline const Color BLANK{ 0,   0,   0,   0 }; // Blank (Transparent)
	inline const Color RED{ 255,   0,   0, 255 };
	inline const Color DARKRED{ 149,   6,   6, 255 };
	inline const Color GREEN{ 0, 255,   0, 255 };
	inline const Color DARKGREEN{ 6,  64,  43, 255 };
	inline const Color FORESTGREEN{ 46, 111,  64, 255 };
	inline const Color BLUE{ 0,   0, 255, 255 };
	inline const Color DARKBLUE{ 17,  17, 132, 255 };
	inline const Color SKYBLUE{ 130, 200, 229, 255 };
	inline const Color GRAY{ 137, 137, 137, 255 };
	inline const Color DARKGRAY{ 53,  62,  67, 255 };
	inline const Color YELLOW{ 255, 222,  33, 255 };
	inline const Color GOLD{ 239, 191,   4, 255 };
	inline const Color MAGENTA{ 255,  29, 206, 255 };
	inline const Color ERROR_COLOR{ 255,   0, 255, 255 };
}