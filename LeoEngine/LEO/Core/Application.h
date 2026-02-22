#pragma once
#include <LEO/Platform/LeoWindow.h>
#include <LEO/Utilities/LeoTimer.h>
#include <LEO/ECS/EntityManager.h>
#include <LEO/ECS/ComponentArray.h>
#include <LEO/ECS/ComponentStoreSparse.h>
#include "LayerStack.h"

namespace leo
{
	class Application
	{
	public:
		Application(const WindowsParameters& win_params);
		~Application();
	public:
		void Run();
		void Stop();
	public:
		void RaiseEvent(Event& event);
	public:
		Window& GetWindow();
		LayerStack& GetLayerStack();
	public:
		static Application& Get();
	private:
		Window m_window;
		LayerStack m_layerStack;
		EntityManager m_entityManager;
	public:
		FrameTimer m_timer;
		bool m_isRunning = false;
	};
}

