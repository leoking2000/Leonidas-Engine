#pragma once
#include <memory>
#include <vector>

#include "Layer.h"
#include "Window/Window.h"

namespace LEO 
{
    class Application final
    {
    public:
        Application(const WindowsParameters& win_params);
        
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;

        ~Application();
    public:
        void Run();
        void Stop();
    public:
        // Layer Management
        void PushLayer(std::unique_ptr<Layer> layer);
        //void PopLayer(Layer* layer);

        template<typename T, typename... Args>
        void PushLayer(Args&&... args)
        {
            static_assert(std::is_base_of<Layer, T>::value, "T must be derived from Layer");
            auto layer = std::make_unique<T>(std::forward<Args>(args)...);
            PushLayer(std::move(layer));
        }
    public:
        static Application& Get();
    private:
        std::vector<std::unique_ptr<Layer>> m_layers;
        bool m_running = false;
    };

    Application& GetApp();
}