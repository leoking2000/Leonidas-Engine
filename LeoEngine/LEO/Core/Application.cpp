#include <LEO/Log/Log.h>
#include <LEO/Graphics/LeoGraphics.h>
#include "Application.h"


namespace leo
{
	static Application* s_Application = nullptr;

	Application::Application(const WindowsParameters& win_params)
		:
		m_window(win_params)
	{
		LEOASSERT(s_Application == nullptr, "Application is already created!, there can be only one Application!");

		s_Application = this;

		leo::WINInitialization();
		m_window.Create();

		leo::GraphicsInitialization();
	}

	Application::~Application()
	{
		s_Application = nullptr;

		m_window.Destroy();
		leo::WINTerminate();
	}

	void Application::Run()
	{
		m_isRunning = true;

		while (m_isRunning)
		{
			m_window.BeginFrame();

			if (m_window.ShouldClose())
			{
				Stop();
				break;
			}

			m_layerStack.ApplyPending();

			f32 dt = m_window.DeltaTime();
			for (auto& layer : m_layerStack)
			{
				layer->OnUpdate(dt);
			}

			m_window.EndFrame();
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
