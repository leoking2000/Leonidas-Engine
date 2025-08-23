#include <raylib/raylib.h>
#include <imgui/rlImGui.h>
#include "Window.h"
#include "Log/Log.h"

// todo: have a direct3D 12 implimitation

namespace LEO::WIN
{
	static LeoColor g_clear_color = LEO_BLACK;

	// Window-related functions //


	// Initialize window and OpenGL context
	void Initialization(WindowsParameters win_params)
	{
		SetTraceLogLevel(LOG_ERROR);

		u32 raylib_flag = 0;
		if (win_params.win_init_flags & FLAG_RESIZABLE)
		{
			raylib_flag |= FLAG_WINDOW_RESIZABLE;
		}
		if (win_params.win_init_flags & FLAG_VSYNC)
		{
			raylib_flag |= FLAG_VSYNC_HINT;
		}
		if (win_params.win_init_flags & FLAG_FULLSCREEN)
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

		LEOLOGINFO("Window created with size: {} x {}", win_params.win_width, win_params.win_height);
	}

	// Initialize window and OpenGL context
	void Initialization(u32 width, u32 height, std::string_view title, u32 flags)
	{
		WindowsParameters params;
		params.win_height = width;
		params.win_height = height;
		params.win_init_flags = flags;
		params.win_title = title.data();

		Initialization(params);
	}

	// Close window and unload OpenGL context
	void Terminate()
	{
		rlImGuiShutdown();
		CloseWindow();

		LEOLOGINFO("Window Closed!");
	}

	// Check if application should close (KEY_ESCAPE pressed or windows close icon clicked)
	bool ShouldClose()
	{
		return WindowShouldClose();
	}

	// Set title for window
	void SetTitle(std::string_view title)
	{
		SetWindowTitle(title.data());
		LEOLOGVERBOSE("Window title set to {}", title.data());
	}


	// Drawing-related functions //

	void SetClearColor(LeoColor color)
	{
		g_clear_color = color;
	}

	// Setup canvas (framebuffer) to start drawing
	void StartFrame()
	{
		BeginDrawing();
		rlImGuiBegin();

		ClearBackground(Color{ g_clear_color.r, g_clear_color.g, g_clear_color.b, g_clear_color.a });
	}

	// End canvas drawing and swap buffers (double buffering)
	void EndFrame()
	{
		rlImGuiEnd();
		EndDrawing();
	}


}