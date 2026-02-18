#include <glm/glm.hpp>

namespace leo
{
	using Color = glm::u8vec4;

	// Some Basic Colors
	#define LEO_WHITE        leo::Color{ 255, 255, 255, 255 }
	#define LEO_BLACK        leo::Color{   0,   0,   0, 255 }
	#define LEO_BLANK        leo::Color{   0,   0,   0,   0 } // Blank (Transparent)
	#define LEO_RED			 leo::Color{ 255,   0,   0, 255 }
	#define LEO_DARKRED		 leo::Color{ 149,   6,   6, 255 }
	#define LEO_GREEN		 leo::Color{   0, 255,   0, 255 }
	#define LEO_DARKGREEN	 leo::Color{   6,  64,  43, 255 }
	#define LEO_FORESTGREEN  leo::Color{  46, 111,  64, 255 }
	#define LEO_BLEU		 leo::Color{   0,   0, 255, 255 }
	#define LEO_DARKBLUE	 leo::Color{  17,  17, 132, 255 }
	#define LEO_SKYBLUE		 leo::Color{ 130, 200, 229, 255 }
	#define LEO_GRAY     	 leo::Color{ 137, 137, 137, 255 }
	#define LEO_DARKGRAY 	 leo::Color{  53,  62,  67, 255 }
	#define LEO_YELLOW 		 leo::Color{ 255, 222,  33, 255 }
	#define LEO_GOLD		 leo::Color{ 239, 191,   4, 255 }
	#define LEO_MAGENTA    	 leo::Color{ 255,  29, 206, 255 }
	#define LEO_ERROR_COLOR	 leo::Color{ 255,   0, 255, 255 }
}