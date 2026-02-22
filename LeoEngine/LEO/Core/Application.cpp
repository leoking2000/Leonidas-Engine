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

		m_window.SetKeyboardCallback([&](int key, int action) {
			switch (action)
			{
			case KEY_PRESS: {
				KeyPressedEvent e;
				e.keyCode = key;
				e.isRepeat = false;
				RaiseEvent(e);
				}
				break;
			case KEY_REPEAT: {
				KeyPressedEvent e;
				e.keyCode = key;
				e.isRepeat = true;
				RaiseEvent(e);
			}
				break;
			case KEY_RELEASE: {
				KeyReleasedEvent e;
				e.keyCode = key;
				RaiseEvent(e);
			}
				break;
			default:
				break;
			}
		});

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

}
