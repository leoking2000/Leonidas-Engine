#pragma once
#include <string>

namespace LEO
{
	typedef enum 
	{
		WIN_FLAG_DEFAULT    =        0,
		WIN_FLAG_RESIZABLE  = (1 << 0),
		WIN_FLAG_VSYNC      = (1 << 1),
		WIN_FLAG_FULLSCREEN = (1 << 2),
		WIN_FLAG_ESC_CLOSE  = (1 << 3),
	} ConfigFlags;

	struct WindowsParameters
	{
		u32          win_width       = 1600;
		u32          win_height      = 900;
		u32          win_init_flags  = WIN_FLAG_DEFAULT;
		const char*  win_title       = "Leonidas Engine";
	};

	typedef glm::u8vec4 Color;

	// Some Basic Colors
	#define LEO_WHITE        LEO::Color{ 255, 255, 255, 255 }
	#define LEO_BLACK        LEO::Color{   0,   0,   0, 255 }
	#define LEO_BLANK        LEO::Color{   0,   0,   0,   0 } // Blank (Transparent)
	#define LEO_RED			 LEO::Color{ 255,   0,   0, 255 }
	#define LEO_DARKRED		 LEO::Color{ 149,   6,   6, 255 }
	#define LEO_GREEN		 LEO::Color{   0, 255,   0, 255 }
	#define LEO_DARKGREEN	 LEO::Color{   6,  64,  43, 255 }
	#define LEO_FORESTGREEN  LEO::Color{  46, 111,  64, 255 }
	#define LEO_BLEU		 LEO::Color{   0,   0, 255, 255 }
	#define LEO_DARKBLUE	 LEO::Color{  17,  17, 132, 255 }
	#define LEO_SKYBLUE		 LEO::Color{ 130, 200, 229, 255 }
	#define LEO_GRAY     	 LEO::Color{ 137, 137, 137, 255 }
	#define LEO_DARKGRAY 	 LEO::Color{  53,  62,  67, 255 }
	#define LEO_YELLOW 		 LEO::Color{ 255, 222,  33, 255 }
	#define LEO_GOLD		 LEO::Color{ 239, 191,   4, 255 }
	#define LEO_MAGENTA    	 LEO::Color{ 255,  29, 206, 255 }
	#define LEO_ERROR_COLOR	 LEO::Color{ 255,   0, 255, 255 }


	// Window-related functions //

	// Initialize window and OpenGL context
	void CreateWindow(WindowsParameters win_params);

	// Initialize window and OpenGL context
	void CreateWindow(u32 width, u32 height, std::string_view title, u32 flags = WIN_FLAG_DEFAULT);

	// Close window and unload OpenGL context
	void DestroyWindow();

	// Check if application should close (KEY_ESCAPE pressed or windows close icon clicked)
	bool ShouldCloseWindow();

	// Set title for window
	void SetWinTitle(std::string_view title);

	// Get current window width
	i32  WinWidth();

	// Get current window height
	i32  WinHeight();


	// Timing-related functions //

	// Set target FPS (maximum)
	void SetFPSTarget(u32 fps);

	// Get time in seconds for last frame drawn (delta time)
	f32  DeltaTime();
	
	// Get elapsed time in seconds since CreateWindow()
	f64  GlobalTime();
	
	// Get current FPS
	u32  CurrentFPS();


	// Drawing-related functions

	// Set background color (framebuffer clear color)
	void SetClearColor(Color color);

	// Setup canvas (framebuffer) to start drawing and clears background
	void StartFrame();

	// End canvas drawing and swap buffers (double buffering)
	void EndFrame();

	// Draw a color-filled circle
	void RenderCircle(const glm::vec2& pos, f32 radius, Color color);

	// Draw a color-filled triangle (vertex in counter-clockwise order!)
	void RenderTriangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, Color color);


	// Input Handling Functions //

	// Keyboard keys
	enum keyboard_key {
		KEY_NONE = 0,                 // key: null
		// Alphanumeric keys

		KEY_APOSTROPHE = 39,          // Key: '
		KEY_COMMA = 44,               // Key: ,
		KEY_MINUS = 45,               // Key: -
		KEY_PERIOD = 46,              // Key: .
		KEY_SLASH = 47,               // Key: /
		KEY_ZERO = 48,                // Key: 0
		KEY_ONE = 49,                 // Key: 1
		KEY_TWO = 50,                 // Key: 2
		KEY_THREE = 51,               // Key: 3
		KEY_FOUR = 52,                // Key: 4
		KEY_FIVE = 53,                // Key: 5
		KEY_SIX = 54,                 // Key: 6
		KEY_SEVEN = 55,               // Key: 7
		KEY_EIGHT = 56,               // Key: 8
		KEY_NINE = 57,                // Key: 9
		KEY_SEMICOLON = 59,           // Key: ;
		KEY_EQUAL = 61,               // Key: =
		KEY_A = 65,                   // Key: A | a
		KEY_B = 66,                   // Key: B | b
		KEY_C = 67,                   // Key: C | c
		KEY_D = 68,                   // Key: D | d
		KEY_E = 69,                   // Key: E | e
		KEY_F = 70,                   // Key: F | f
		KEY_G = 71,                   // Key: G | g
		KEY_H = 72,                   // Key: H | h
		KEY_I = 73,                   // Key: I | i
		KEY_J = 74,                   // Key: J | j
		KEY_K = 75,                   // Key: K | k
		KEY_L = 76,                   // Key: L | l
		KEY_M = 77,                   // Key: M | m
		KEY_N = 78,                   // Key: N | n
		KEY_O = 79,                   // Key: O | o
		KEY_P = 80,                   // Key: P | p
		KEY_Q = 81,                   // Key: Q | q
		KEY_R = 82,                   // Key: R | r
		KEY_S = 83,                   // Key: S | s
		KEY_T = 84,                   // Key: T | t
		KEY_U = 85,                   // Key: U | u
		KEY_V = 86,                   // Key: V | v
		KEY_W = 87,                   // Key: W | w
		KEY_X = 88,                   // Key: X | x
		KEY_Y = 89,                   // Key: Y | y
		KEY_Z = 90,                   // Key: Z | z
		KEY_LEFT_BRACKET = 91,        // Key: [
		KEY_BACKSLASH = 92,           // Key: '\'
		KEY_RIGHT_BRACKET = 93,       // Key: ]
		KEY_GRAVE = 96,               // Key: `
		// Function keys
		KEY_SPACE = 32,               // Key: Space
		KEY_ESCAPE = 256,             // Key: Esc
		KEY_ENTER = 257,              // Key: Enter
		KEY_TAB = 258,                // Key: Tab
		KEY_BACKSPACE = 259,          // Key: Backspace
		KEY_INSERT = 260,             // Key: Ins
		KEY_DELETE = 261,             // Key: Del
		KEY_RIGHT = 262,              // Key: Cursor right
		KEY_LEFT = 263,               // Key: Cursor left
		KEY_DOWN = 264,               // Key: Cursor down
		KEY_UP = 265,                 // Key: Cursor up
		KEY_PAGE_UP = 266,            // Key: Page up
		KEY_PAGE_DOWN = 267,          // Key: Page down
		KEY_HOME = 268,               // Key: Home
		KEY_END = 269,                // Key: End
		KEY_CAPS_LOCK = 280,          // Key: Caps lock
		KEY_SCROLL_LOCK = 281,        // Key: Scroll down
		KEY_NUM_LOCK = 282,           // Key: Num lock
		KEY_PRINT_SCREEN = 283,       // Key: Print screen
		KEY_PAUSE = 284,              // Key: Pause
		KEY_F1 = 290,                 // Key: F1
		KEY_F2 = 291,                 // Key: F2
		KEY_F3 = 292,                 // Key: F3
		KEY_F4 = 293,                 // Key: F4
		KEY_F5 = 294,                 // Key: F5
		KEY_F6 = 295,                 // Key: F6
		KEY_F7 = 296,                 // Key: F7
		KEY_F8 = 297,                 // Key: F8
		KEY_F9 = 298,                 // Key: F9
		KEY_F10 = 299,                // Key: F10
		KEY_F11 = 300,                // Key: F11
		KEY_F12 = 301,                // Key: F12
		KEY_LEFT_SHIFT = 340,         // Key: Shift left
		KEY_LEFT_CONTROL = 341,       // Key: Control left
		KEY_LEFT_ALT = 342,           // Key: Alt left
		KEY_LEFT_SUPER = 343,         // Key: Super left
		KEY_RIGHT_SHIFT = 344,        // Key: Shift right
		KEY_RIGHT_CONTROL = 345,      // Key: Control right
		KEY_RIGHT_ALT = 346,          // Key: Alt right
		KEY_RIGHT_SUPER = 347,        // Key: Super right
		KEY_KB_MENU = 348,            // Key: KB menu
	};

	enum mouse_key {
		MOUSE_BUTTON_LEFT = 0,       // Mouse button left
		MOUSE_BUTTON_RIGHT = 1,      // Mouse button right
		MOUSE_BUTTON_MIDDLE = 2,     // Mouse button middle (pressed wheel)
		MOUSE_BUTTON_SIDE = 3,       // Mouse button side (advanced mouse device)
		MOUSE_BUTTON_EXTRA = 4,      // Mouse button extra (advanced mouse device)
		MOUSE_BUTTON_FORWARD = 5,    // Mouse button forward (advanced mouse device)
		MOUSE_BUTTON_BACK = 6,       // Mouse button back (advanced mouse device)
	};

	// Check if a key is being pressed
	bool KeyDown(keyboard_key key);

	// Check if a mouse button has been pressed once
	bool MouseButtonPressed(mouse_key key);

	// Get mouse position XY
	glm::vec2 MousePosition();
}