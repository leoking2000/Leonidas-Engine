#include <algorithm>
#include "LEO/Log/Log.h"
#include "Window/Window.h"
#include "Application.h"


namespace LEO 
{
    Application* g_app = nullptr;

    Application& Application::Get()
    {
        return *g_app;
    }

    Application& GetApp()
    {
        return Application::Get();
    }

    Application::Application(const WindowsParameters& win_params)
    {
        LEOASSERT(g_app == nullptr, "Application already exists!");
        g_app = this;

        LEO::CreateWindow(win_params);
    }

    Application::~Application()
    {
        LEO::DestroyWindow();
        g_app = nullptr;
    }

    void Application::Run()
    {
        m_running = true;
        while (m_running)
        {
            LEO::StartFrame();

            for (auto& layer : m_layers)
            {
                layer->OnUpdate();
            }

            for (auto& layer : m_layers)
            {
                layer->OnRender();
                layer->OnRenderImGui();
            }

            if(LEO::ShouldCloseWindow()){
                Stop();
            }

            LEO::EndFrame();
        }
    }

    void Application::Stop()
    {
        m_running = false;
    }

    void Application::PushLayer(std::unique_ptr<Layer> layer)
    {
        m_layers.emplace_back(std::move(layer));
        m_layers.back()->OnCreate();
    }

    //void Application::PopLayer(Layer* layer)
    //{
    //    auto it = std::find_if(m_layers.begin(), m_layers.end(),
    //        [layer](const std::unique_ptr<Layer>& ptr) { return ptr.get() == layer; });
//
    //    if (it != m_layers.end())
    //    {
    //        (*it)->OnDestroy();
    //        m_layers.erase(it);
    //    }
    //}
}
