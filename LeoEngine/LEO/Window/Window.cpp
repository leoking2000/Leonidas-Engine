#include <raylib.h>
#include <imgui/rlImGui.h>
#include "Window.h"
#include "Log/Log.h"


namespace LEO
{
	static Color g_clear_color = LEO_BLACK;

	// Window-related functions //

	// Initialize window and OpenGL context
	void CreateWindow(WindowsParameters win_params)
	{
		SetTraceLogLevel(LOG_ERROR);

		u32 raylib_flag = 0;
		if (win_params.win_init_flags & WIN_FLAG_RESIZABLE)
		{
			raylib_flag |= FLAG_WINDOW_RESIZABLE;
		}
		if (win_params.win_init_flags & WIN_FLAG_VSYNC)
		{
			raylib_flag |= FLAG_VSYNC_HINT;
		}
		if (win_params.win_init_flags & WIN_FLAG_FULLSCREEN)
		{
			raylib_flag |= FLAG_FULLSCREEN_MODE;
			win_params.win_width = GetScreenWidth();
			win_params.win_height = GetScreenHeight();
		}
		SetConfigFlags(raylib_flag);

		InitWindow(win_params.win_width, win_params.win_height, win_params.win_title);

		if (!IsWindowReady())
		{
			LEOLOGFATAL("Window Failed to be created!!!");
			std::terminate();
		}

		rlImGuiSetup(true);

		SetExitKey(KEY_NULL);
		if (win_params.win_init_flags & WIN_FLAG_ESC_CLOSE)
		{
			SetExitKey(KEY_ESCAPE);
		}

		LEOLOGINFO("Window created with size: {} x {}", win_params.win_width, win_params.win_height);
	}

	// Initialize window and OpenGL context
	void CreateWindow(u32 width, u32 height, std::string_view title, u32 flags)
	{
		WindowsParameters params;
		params.win_width = width;
		params.win_height = height;
		params.win_init_flags = flags;
		params.win_title = title.data();

		CreateWindow(params);
	}

	// Close window and unload OpenGL context
	void DestroyWindow()
	{
		rlImGuiShutdown();
		CloseWindow();

		LEOLOGINFO("Window Closed!");
	}

	// Check if application should close (KEY_ESCAPE pressed or windows close icon clicked)
	bool ShouldCloseWindow()
	{
		return WindowShouldClose();
	}

	// Set title for window
	void SetWinTitle(std::string_view title)
	{
		SetWindowTitle(title.data());
		LEOLOGVERBOSE("Window title set to {}", title.data());
	}

	i32 WinWidth()
	{
		return GetScreenWidth();
	}

	i32 WinHeight()
	{
		return GetScreenHeight();
	}

	// Timing-related functions //

	// Set target FPS (maximum)
	void SetFPSTarget(u32 fps)
	{
		SetTargetFPS(fps);
	}

	// Get time in seconds for last frame drawn (delta time)
	f32  DeltaTime()
	{
		return GetFrameTime();
	}

	// Get elapsed time in seconds since CreateWindow()
	f64  GlobalTime()
	{
		return GetTime();
	}

	// Get current FPS
	u32  CurrentFPS()
	{
		return GetFPS();
	}


	// Drawing-related functions //

	void SetClearColor(Color color)
	{
		g_clear_color = color;
	}

	// Setup canvas (framebuffer) to start drawing and clears background
	void StartFrame()
	{
		BeginDrawing();
		rlImGuiBegin();

		ClearBackground({ g_clear_color.r, g_clear_color.g, g_clear_color.b, g_clear_color.a });
	}

	// End canvas drawing and swap buffers (double buffering)
	void EndFrame()
	{
		rlImGuiEnd();
		EndDrawing();
	}

	// Draw a color-filled circle
	void RenderCircle(const glm::vec2& pos, f32 radius, Color color)
	{
		DrawCircle(int(pos.x), int(pos.y), radius, { color.r, color.g, color.b, color.a });
	}

	// Draw a color-filled triangle (vertex in counter-clockwise order!)
	void RenderTriangle(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, Color color)
	{
		DrawTriangle({ a.x, a.y }, { b.x, b.y }, { c.x, c.y }, { color.r, color.g, color.b, color.a });
	}

	// Input Handling Functions //

	// Check if a key is being pressed
	bool KeyDown(keyboard_key key)
	{
		return IsKeyDown((int)key);
	}

	// Check if a mouse button has been pressed once
	bool MouseButtonPressed(mouse_key key)
	{
		return IsMouseButtonPressed((int)key);
	}

	// Get mouse position XY
	glm::vec2 MousePosition()
	{
		Vector2 v = GetMousePosition();
		return { v.x, v.y };
	}


}