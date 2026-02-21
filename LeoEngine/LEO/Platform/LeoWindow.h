#pragma once
#include <functional>
#include <glm/glm.hpp>
#include <array>
#include <LEO/Utilities/LeoTypes.h>
#include <LEO/Utilities/LeoTimer.h>
#include <LEO/Utilities/LeoColors.h>
#include "Keys.h"

namespace leo
{
	typedef enum
	{
		WIN_FLAG_DEFAULT    = 0,
		WIN_FLAG_RESIZABLE  = (1 << 0),
		WIN_FLAG_VSYNC      = (1 << 1),
		WIN_FLAG_ESC_CLOSE  = (1 << 3),
	} ConfigFlags;

	struct WindowsParameters
	{
		u32          width       = 1600;
		u32          height      = 900;
		const char*  title       = "Leonidas Engine";
		u32          init_flags  = WIN_FLAG_DEFAULT;
	};

	void WINInitialization();
	void WINTerminate();

	class Window final
	{
	public:
		Window(WindowsParameters win_params);
		Window(u32 width, u32 height, const std::string& title, u32 flags = WIN_FLAG_DEFAULT);

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
	public:
		void Create();
		void Destroy();
	public:
		// Check if window should close (KEY_ESCAPE pressed or windows close icon clicked)
		bool ShouldClose() const;
		// Setup canvas (framebuffer) to start drawing
		void BeginFrame();
		// End canvas drawing and swap buffers (double buffering) and pull events
		void EndFrame();
		// Close window
		void Close();
	public:
		// Get time in seconds for last frame drawn (delta time)
		f32	DeltaTime() const;
		// Get elapsed time in seconds since App creation
		f32	Time() const;
		// Get current FPS
		u32 FPS() const;
	public:
		// Set title for window
		void SetTitle(const std::string& title) const;
		// Enable/disable Vsync
		void SetVsync(bool vsync);
	public:
		// Get current window Size (width, height) in pixels
		glm::uvec2 Size() const;
		// Get current window half Size (width/2, height/2) in pixels
		glm::uvec2 HalfSize() const;
		// Get mouse position XY
		glm::vec2  MousePos()   const;
		// mouse cursur visibility
		void SetMouseVisibility(bool visible);
	public:
		// Check if a key is being pressed 
		bool KeyDown(int key) const;
		// will return true only in the first frame the key was pressed the user will have to stop pressing the key and then press it again to activate the action
		bool KeyIsPressAsButton(int key) const;
		// Check if a mouse button is being pressed
		bool MouseButtonDown(int key) const;
	public:
		// Draws a filled circle in pixel space
		void DrawCircle(float centerX, float centerY, float radius, const Color& color, int segments = 32) const;
	public:
		using WindowResizeCallback = std::function<void(int, int)>;
		void SetResizeCallback(WindowResizeCallback resize_callback);

		// first input is the keycode, second is the action(press, relesed, repeat)
		using ButtonEventCallback = std::function<void(int, int)>;
		void SetKeyboardCallback(ButtonEventCallback key_callback);
		void SetMouseButtonCallBack(ButtonEventCallback mouse_key_callback);

		using MouseMoveCallback = std::function<void(float, float)>;
		void SetMouseMoveCallback(MouseMoveCallback mouse_move_callback);
	public:
		struct WinData
		{
			WindowsParameters    params;
			ButtonEventCallback  keyboardCallback;
			ButtonEventCallback  mouseKeyCallback;
			MouseMoveCallback    mouseMoveCallback;
			WindowResizeCallback windowResizeCallback;
		};
	private:
		struct GLFWwindow* m_window = nullptr;
		WinData m_data = {};
		FrameTimer m_timer;
		mutable std::array<bool, 512u> m_keyStates{}; // use in KeyIsPressAsButton
	};
}
