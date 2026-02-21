#include <thread>
#include <glad/glad.h>
#include <LEO/Graphics/LeoGraphics.h>
#include <GLFW/glfw3.h> 
#include <LEO/Log/Log.h>
#include "LeoWindow.h"

namespace leo
{
	static bool g_glfwInnit = false;

	void glfw_error_callback(int code, const char* description)
	{
		LEOLOGERROR("GLFW error {} : {}", code, description);
	}

	void WINInitialization()
	{
		LEOASSERT(g_glfwInnit == false, "WIN system is already Initialized.");

		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
		{
			LEOASSERT(false, "GLFW init failed.");
		}

		// Hits
		// OpenGL version 4.6
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 0);

		g_glfwInnit = true;
	}

	void WINTerminate()
	{
		LEOASSERT(g_glfwInnit == true, "WIN system has not been Initialized.");
		g_glfwInnit = false;
		glfwTerminate();
	}

	//-----------------------------------------------------------

	void glfw_window_size_callback(GLFWwindow* glfw_window, int width, int height)
	{
		Window::WinData* data = reinterpret_cast<Window::WinData*>(glfwGetWindowUserPointer(glfw_window));

		data->params.width = (u32)width;
		data->params.height = (u32)height;
		glViewport(0, 0, width, height);

		if (data->windowResizeCallback)
		{
			data->windowResizeCallback(width, height);
		}
	}

	void glfw_key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods)
	{
		Window::WinData* data = reinterpret_cast<Window::WinData*>(glfwGetWindowUserPointer(glfw_window));

		if (data->keyboardCallback)
		{
			data->keyboardCallback(key, action);
		}
	}

	void glfw_mouse_button_callback(GLFWwindow* glfw_window, int button, int action, int mods)
	{
		Window::WinData* data = reinterpret_cast<Window::WinData*>(glfwGetWindowUserPointer(glfw_window));
		if (data->mouseKeyCallback)
		{
			data->mouseKeyCallback(button, action);
		}
	}

	static void glfw_cursor_position_callback(GLFWwindow* glfw_window, double xpos, double ypos)
	{
		Window::WinData* data = reinterpret_cast<Window::WinData*>(glfwGetWindowUserPointer(glfw_window));
		if (data->mouseMoveCallback)
		{
			data->mouseMoveCallback((float)xpos, (float)ypos);
		}
	}

	//-----------------------------------------------------------

	Window::Window(WindowsParameters win_params)
	{
		m_data.params = win_params;
	}

	Window::Window(u32 width, u32 height, const std::string& title, u32 flags)
		:
		Window(leo::WindowsParameters{width, height, title.c_str(), flags})
	{
	}

	//-----------------------------------------------------------

	void Window::Create()
	{
		LEOASSERT(g_glfwInnit == true, "WIN system needs to be Initialized to create window call LEO::WINInitialization()");

		// window hits
		glfwWindowHint(GLFW_RESIZABLE, m_data.params.init_flags & WIN_FLAG_RESIZABLE ? GL_TRUE : GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		// TODO: add fullscreen support

		// create the window
		m_window = glfwCreateWindow(m_data.params.width, m_data.params.height, m_data.params.title, NULL, NULL);

		if (m_window == nullptr)
		{
			LEOLOGERROR("Window creation failed.");
		}

		// set user pointer (used by the callbacks)
		glfwSetWindowUserPointer(m_window, &m_data);

		// set the glfw callbacks
		glfwSetWindowSizeCallback(m_window, glfw_window_size_callback);
		glfwSetKeyCallback(m_window, glfw_key_callback);
		glfwSetMouseButtonCallback(m_window, glfw_mouse_button_callback);
		glfwSetCursorPosCallback(m_window, glfw_cursor_position_callback);

		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(m_data.params.init_flags & WIN_FLAG_VSYNC ? 1 : 0);
	}

	void Window::Destroy()
	{
		glfwDestroyWindow(m_window);
	}

	//-----------------------------------------------------------

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_window);
	}

	void Window::Close()
	{
		glfwSetWindowShouldClose(m_window, true);
	}

	void Window::BeginFrame()
	{
		glfwPollEvents();
		m_timer.Tick();
	}

	void Window::EndFrame()
	{
		glfwSwapBuffers(m_window);

		if (m_data.params.init_flags & WIN_FLAG_ESC_CLOSE)
		{
			if (KeyIsPressAsButton(KEY_ESCAPE))
			{
				Close(); // close window
			}
		}
	}

	//-----------------------------------------------------------

	f32 leo::Window::DeltaTime() const
	{
		return m_timer.DeltaTime();
	}

	f32 Window::Time() const
	{
		return (f32)m_timer.TotalTime();
	}

	u32 leo::Window::FPS() const
	{
		return (u32)m_timer.FPS();
	}

	//-----------------------------------------------------------

	void Window::SetTitle(const std::string& title) const
	{
		glfwSetWindowTitle(m_window, title.c_str());
	}

	void Window::SetVsync(bool vsync)
	{
		glfwSwapInterval(vsync ? 1 : 0);
	}

	//-----------------------------------------------------------

	glm::uvec2 Window::Size() const
	{
		return glm::uvec2(m_data.params.width, m_data.params.height);
	}

	glm::uvec2 Window::HalfSize() const
	{
		return glm::uvec2(m_data.params.width / 2, m_data.params.height / 2);
	}

	//-----------------------------------------------------------

	glm::vec2 Window::MousePos() const
	{
		double xpos, ypos;
		glfwGetCursorPos(m_window, &xpos, &ypos);

		return glm::vec2((float)xpos, (float)ypos);
	}

	void Window::SetMouseVisibility(bool visible)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	//-----------------------------------------------------------

	bool Window::KeyDown(int key) const
	{
		return glfwGetKey(m_window, key) == GLFW_PRESS;
	}

	bool Window::KeyIsPressAsButton(int key) const
	{
		if (key < 0 || key >= 512) 
		{
			return false;
		}

		if (KeyDown(key) && m_keyStates[key] == false)
		{
			m_keyStates[key] = true;
			return true;
		}
		else if (!KeyDown(key))
		{
			m_keyStates[key] = false;
		}

		return false;
	}

	bool Window::MouseButtonDown(int key) const
	{
		return glfwGetMouseButton(m_window, key) == GLFW_PRESS;
	}

	//-----------------------------------------------------------

	void Window::DrawCircle(float centerX, float centerY, float radius, const Color& color, int segments) const
	{
		std::vector<glm::vec2> vertices;
		vertices.emplace_back(centerX, centerY); // center vertex

		for (int i = 0; i <= segments; ++i)
		{
			float theta = 2.0f * 3.14159265f * float(i) / float(segments);
			float x = centerX + cosf(theta) * radius;
			float y = centerY + sinf(theta) * radius;
			vertices.emplace_back(x, y);
		}

		// 2. Convert pixels to NDC [-1,1]
		for (auto& v : vertices)
		{
			v.x = (v.x / m_data.params.width) * 2.0f - 1.0f;
			v.y = 1.0f - (v.y / m_data.params.height) * 2.0f; // flip Y
		}

		// 3. Create VAO + VBO
		leo::VertexBuffer vb(vertices.data(), u32(sizeof(glm::vec2) * vertices.size()));
		leo::ElementType arr[1] = { leo::ElementType::FLOAT2 };
		leo::Layout<1> layout(arr);

		leo::VertexArray vao;
		vao.AddBuffer(std::move(vb), layout);

		// 4. Create a simple shader (or reuse a shader)
		static leo::ShaderProgram circleShader(RESOURCES_PATH"Shaders/CircleShader/basic");
		circleShader.Bind();

		glm::vec4 normalizedColor = glm::vec4(color) / 255.0f;
		circleShader.SetUniform("u_Color", normalizedColor);

		// 5. Draw
		glBindVertexArray(vao.ID());
		glDrawArrays(GL_TRIANGLE_FAN, 0, GLsizei(vertices.size()));
		glBindVertexArray(0);

		circleShader.UnBind();
	}

	//-----------------------------------------------------------

	void Window::SetResizeCallback(WindowResizeCallback resize_callback)
	{
		m_data.windowResizeCallback = resize_callback;
	}

	void Window::SetKeyboardCallback(ButtonEventCallback key_callback)
	{
		m_data.keyboardCallback = key_callback;
	}

	void Window::SetMouseButtonCallBack(ButtonEventCallback mouse_key_callback)
	{
		m_data.mouseKeyCallback = mouse_key_callback;
	}

	void Window::SetMouseMoveCallback(MouseMoveCallback mouse_move_callback)
	{
		m_data.mouseMoveCallback = mouse_move_callback;
	}
}
