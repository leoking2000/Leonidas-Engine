#pragma once
#include <string>
#include "Utilities/Types.h"

namespace LEO::WIN
{
	typedef enum 
	{
		FLAG_DEFAULT    = 0x00000000,
		FLAG_RESIZABLE  = 0x00000002,
		FLAG_VSYNC      = 0x00000004,
		FLAG_FULLSCREEN = 0x00000008,
	} ConfigFlags;

	struct WindowsParameters
	{
		u32          win_width       = 1600;
		u32          win_height      = 900;
		u32          win_init_flags  = FLAG_DEFAULT;
		const char*  win_title       = "Leonidas Engine";
	};

	typedef glm::u8vec4 LeoColor;

	// Some Basic Colors
	#define LEO_WHITE        LEO::WIN::LeoColor{ 255, 255, 255, 255 }
	#define LEO_BLACK        LEO::WIN::LeoColor{   0,   0,   0, 255 }
	#define LEO_BLANK        LEO::WIN::LeoColor{   0,   0,   0,   0 } // Blank (Transparent)
	#define LEO_RED			 LEO::WIN::LeoColor{ 255,   0,   0, 255 }
	#define LEO_DARKRED		 LEO::WIN::LeoColor{ 149,   6,   6, 255 }
	#define LEO_GREEN		 LEO::WIN::LeoColor{   0, 255,   0, 255 }
	#define LEO_DARKGREEN	 LEO::WIN::LeoColor{   6,  64,  43, 255 }
	#define LEO_FORESTGREEN  LEO::WIN::LeoColor{  46, 111,  64, 255 }
	#define LEO_BLEU		 LEO::WIN::LeoColor{   0,   0, 255, 255 }
	#define LEO_DARKBLUE	 LEO::WIN::LeoColor{  17,  17, 132, 255 }
	#define LEO_SKYBLUE		 LEO::WIN::LeoColor{ 130, 200, 229, 255 }
	#define LEO_GRAY     	 LEO::WIN::LeoColor{ 137, 137, 137, 255 }
	#define LEO_DARKGRAY 	 LEO::WIN::LeoColor{  53,  62,  67, 255 }
	#define LEO_YELLOW 		 LEO::WIN::LeoColor{ 255, 222,  33, 255 }
	#define LEO_GOLD		 LEO::WIN::LeoColor{ 239, 191,   4, 255 }
	#define LEO_MAGENTA    	 LEO::WIN::LeoColor{ 255,  29, 206, 255 }
	#define LEO_ERROR_COLOR	 LEO::WIN::LeoColor{ 255,   0, 255, 255 }


	// Window-related functions

	void Initialization(WindowsParameters win_params);                                                 // Initialize window and OpenGL context
	void Initialization(u32 width, u32 height, std::string_view title, u32 flags = FLAG_DEFAULT);      // Initialize window and OpenGL context
	void Terminate();                                                                                  // Close window and unload OpenGL context																                              
	bool ShouldClose();                                                                                // Check if application should close (KEY_ESCAPE pressed or windows close icon clicked)
	void SetTitle(std::string_view title);                                                             // Set title for window
	i32  Width();                                                                                      // Get current window width
	i32  Height();                                                                                     // Get current window height
																	                                   
	// Drawing-related functions									                                   
	void SetClearColor(LeoColor color);                                                                // Set background color (framebuffer clear color)
	void StartFrame();                                                                                 // Setup canvas (framebuffer) to start drawing
	void EndFrame();                                                                                   // End canvas drawing and swap buffers (double buffering)
	void RenderCircle(const glm::vec2& pos, f32 radius, LeoColor color);                               // Draw a color-filled circle
	void RenderTriangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, LeoColor color);   // Draw a color-filled triangle (vertex in counter-clockwise order!)
}