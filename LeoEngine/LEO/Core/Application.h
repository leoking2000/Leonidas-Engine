#pragma once
#include <LEO/Platform/LeoWindow.h>
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
		bool m_isRunning = false;
		LayerStack m_layerStack;
	};
}

