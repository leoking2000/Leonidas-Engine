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
                layer->OnCreate();
                m_layerStack.push_back(std::move(layer));
            }
        }
        m_toPush.clear(); // Clear the push queue

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
}