#include <LEO/Log/Log.h>
#include <LEO/Graphics/LeoGraphics.h>
#include "Application.h"


namespace leo
{
	static Application* s_Application = nullptr;

	Application::Application(const WindowsParameters& win_params)
		:
		m_window(win_params, false)
	{
		LEOASSERT(s_Application == nullptr, "Application is already created!, there can be only one Application!");

		s_Application = this;

		leo::WINInitialization();
		m_window.Create();

		SetWindowCallbacks();

		leo::GraphicsInitialization();
	}

	Application::~Application()
	{
		s_Application = nullptr;

		m_layerStack.Clean();

		m_window.Destroy();
		leo::WINTerminate();
	}

	void Application::Run()
	{
		m_isRunning = true;

		while (m_isRunning)
		{
			m_window.PollEvents();
			m_timer.Tick();

			if (m_window.ShouldClose())
			{
				Stop();
				break;
			}

			m_layerStack.ApplyPending();

			f32 dt = m_timer.DeltaTime();
			for (auto& layer : m_layerStack)
			{
				layer->OnUpdate(dt);
			}

			m_entityManager.Update(dt);

			m_window.SwapBuffers();
		}

	}

	void Application::Stop()
	{
		m_isRunning = false;
	}

	void Application::RaiseEvent(Event& event)
	{
		for (auto& layer : m_layerStack)
		{
			layer->OnEvent(event);
		}
		m_layerStack.ApplyPending();
	}

	Window& Application::GetWindow()
	{
		return m_window;
	}

	LayerStack& Application::GetLayerStack()
	{
		return m_layerStack;
	}

	Application& Application::Get()
	{
		return *s_Application;
	}

    void Application::SetWindowCallbacks()
    {
        // ---------------- Window Resize ----------------
        m_window.SetResizeCallback([this](int width, int height){
            WindowResizeEvent event;
            event.width = static_cast<u32>(width);
            event.height = static_cast<u32>(height);

            RaiseEvent(event);
        });

        // ---------------- Keyboard ----------------
        m_window.SetKeyboardCallback([this](int key, int action){
            switch (action){
                case KEY_PRESS:
                {
                    KeyPressedEvent event;
                    event.keyCode = key;
                    event.isRepeat = false;
                    RaiseEvent(event);
                    break;
                }

                case KEY_REPEAT:
                {
                    KeyPressedEvent event;
                    event.keyCode = key;
                    event.isRepeat = true;
                    RaiseEvent(event);
                    break;
                }

                case KEY_RELEASE:
                {
                    KeyReleasedEvent event;
                    event.keyCode = key;
                    RaiseEvent(event);
                    break;
                }
            }
         });

        // ---------------- Mouse Button ----------------
        m_window.SetMouseButtonCallBack([this](int button, int action){
            glm::vec2 mousePos = m_window.MousePos();

            if (action == KEY_PRESS)
            {
                MouseButtonPressedEvent event;
                event.code = button;
                event.x = mousePos.x;
                event.y = mousePos.y;
                RaiseEvent(event);
            }
            else if (action == KEY_RELEASE)
            {
                MouseButtonReleasedEvent event;
                event.code = button;
                event.x = mousePos.x;
                event.y = mousePos.y;
                RaiseEvent(event);
            }
        });

        // ---------------- Mouse Move ----------------
        m_window.SetMouseMoveCallback([this](float x, float y){
            MouseMovedEvent event;
            event.x = x;
            event.y = y;

            RaiseEvent(event);
        });
    }

}
