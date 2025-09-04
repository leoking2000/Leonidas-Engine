#pragma once
#include <string>

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


	// Window-related functions //

	// Initialize window and OpenGL context
	void CreateWindow(WindowsParameters win_params);

	// Initialize window and OpenGL context
	void CreateWindow(u32 width, u32 height, std::string_view title, u32 flags = FLAG_DEFAULT);    

	// Close window and unload OpenGL context
	void DestroyWindow();

	// Check if application should close (KEY_ESCAPE pressed or windows close icon clicked)
	bool ShouldClose();

	// Set title for window
	void SetTitle(std::string_view title);

	// Get current window width
	i32  Width();

	// Get current window height
	i32  Height();


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
	void SetClearColor(LeoColor color);

	// Setup canvas (framebuffer) to start drawing and clears background
	void StartFrame();

	// End canvas drawing and swap buffers (double buffering)
	void EndFrame();

	// Draw a color-filled circle
	void RenderCircle(const glm::vec2& pos, f32 radius, LeoColor color);

	// Draw a color-filled triangle (vertex in counter-clockwise order!)
	void RenderTriangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, LeoColor color);


	// Input Handling Functions //

	// Keyboard keys
	enum keyboard_key {
		NONE = 0,                 // key: null
		// Alphanumeric keys

		APOSTROPHE = 39,          // Key: '
		COMMA = 44,               // Key: ,
		MINUS = 45,               // Key: -
		PERIOD = 46,              // Key: .
		SLASH = 47,               // Key: /
		ZERO = 48,                // Key: 0
		ONE = 49,                 // Key: 1
		TWO = 50,                 // Key: 2
		THREE = 51,               // Key: 3
		FOUR = 52,                // Key: 4
		FIVE = 53,                // Key: 5
		SIX = 54,                 // Key: 6
		SEVEN = 55,               // Key: 7
		EIGHT = 56,               // Key: 8
		NINE = 57,                // Key: 9
		SEMICOLON = 59,           // Key: ;
		EQUAL = 61,               // Key: =
		A = 65,                   // Key: A | a
		B = 66,                   // Key: B | b
		C = 67,                   // Key: C | c
		D = 68,                   // Key: D | d
		E = 69,                   // Key: E | e
		F = 70,                   // Key: F | f
		G = 71,                   // Key: G | g
		H = 72,                   // Key: H | h
		I = 73,                   // Key: I | i
		J = 74,                   // Key: J | j
		K = 75,                   // Key: K | k
		L = 76,                   // Key: L | l
		M = 77,                   // Key: M | m
		N = 78,                   // Key: N | n
		O = 79,                   // Key: O | o
		P = 80,                   // Key: P | p
		Q = 81,                   // Key: Q | q
		R = 82,                   // Key: R | r
		S = 83,                   // Key: S | s
		T = 84,                   // Key: T | t
		U = 85,                   // Key: U | u
		V = 86,                   // Key: V | v
		W = 87,                   // Key: W | w
		X = 88,                   // Key: X | x
		Y = 89,                   // Key: Y | y
		Z = 90,                   // Key: Z | z
		LEFT_BRACKET = 91,        // Key: [
		BACKSLASH = 92,           // Key: '\'
		RIGHT_BRACKET = 93,       // Key: ]
		GRAVE = 96,               // Key: `
		// Function keys
		SPACE = 32,               // Key: Space
		ESCAPE = 256,             // Key: Esc
		ENTER = 257,              // Key: Enter
		TAB = 258,                // Key: Tab
		BACKSPACE = 259,          // Key: Backspace
		INSERT = 260,             // Key: Ins
		DELETE = 261,             // Key: Del
		RIGHT = 262,              // Key: Cursor right
		LEFT = 263,               // Key: Cursor left
		DOWN = 264,               // Key: Cursor down
		UP = 265,                 // Key: Cursor up
		PAGE_UP = 266,            // Key: Page up
		PAGE_DOWN = 267,          // Key: Page down
		HOME = 268,               // Key: Home
		END = 269,                // Key: End
		CAPS_LOCK = 280,          // Key: Caps lock
		SCROLL_LOCK = 281,        // Key: Scroll down
		NUM_LOCK = 282,           // Key: Num lock
		PRINT_SCREEN = 283,       // Key: Print screen
		PAUSE = 284,              // Key: Pause
		F1 = 290,                 // Key: F1
		F2 = 291,                 // Key: F2
		F3 = 292,                 // Key: F3
		F4 = 293,                 // Key: F4
		F5 = 294,                 // Key: F5
		F6 = 295,                 // Key: F6
		F7 = 296,                 // Key: F7
		F8 = 297,                 // Key: F8
		F9 = 298,                 // Key: F9
		F10 = 299,                // Key: F10
		F11 = 300,                // Key: F11
		F12 = 301,                // Key: F12
		LEFT_SHIFT = 340,         // Key: Shift left
		LEFT_CONTROL = 341,       // Key: Control left
		LEFT_ALT = 342,           // Key: Alt left
		LEFT_SUPER = 343,         // Key: Super left
		RIGHT_SHIFT = 344,        // Key: Shift right
		RIGHT_CONTROL = 345,      // Key: Control right
		RIGHT_ALT = 346,          // Key: Alt right
		RIGHT_SUPER = 347,        // Key: Super right
		KB_MENU = 348,            // Key: KB menu
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