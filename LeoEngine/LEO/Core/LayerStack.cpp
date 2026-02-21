#include "LayerStack.h"


namespace leo
{
    void LayerStack::ApplyPending()
    {
        // Add queued layers
        while (!m_toPush.empty()) {
            auto pending = std::move(m_toPush);
            m_toPush.clear();

            for (auto& layer : pending) {
                layer->OnCreate(); // OnCreate can PushLayer
                m_layerStack.push_back(std::move(layer));
            }
        }

        // Remove queued layers
        for (Layer* layerToRemove : m_toRemove)
        {
            auto it = std::find_if(
                m_layerStack.begin(), m_layerStack.end(),
                [layerToRemove](const std::unique_ptr<Layer>& l) { return l.get() == layerToRemove; }
            );

            if (it != m_layerStack.end())
            {
                (*it)->OnDestroy();            // Clean up
                m_layerStack.erase(it);        // Remove from stack, unique_ptr deletes automatically
            }
        }
        m_toRemove.clear(); // Clear the remove queue
    }

    void LayerStack::Clean()
    {
        ApplyPending();

        for (auto& layer : m_layerStack)
        {
            layer->OnDestroy();
        }
        m_layerStack.clear();
    }
}