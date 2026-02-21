#pragma once
#include <vector>
#include <memory>
#include "Layer.h"


namespace leo
{
	class LayerStack
	{
	public:
		template<typename TLayer>
			requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer()
		{
			m_toPush.push_back(std::make_unique<TLayer>());
		}

		template<typename TLayer>
			requires(std::is_base_of_v<Layer, TLayer>)
		TLayer* GetLayer()
		{
			for (const auto& layer : m_layerStack)
			{
				if (auto casted = dynamic_cast<TLayer*>(layer.get()))
					return casted;
			}
			return nullptr;
		}
		template<typename TLayer>
			requires(std::is_base_of_v<Layer, TLayer>)
		void RemoveLayer()
		{
			Layer* layer = GetLayer<TLayer>();
			if (layer != nullptr) {
				m_toRemove.emplace_back(layer);
			}		
		}
	public:
		void ApplyPending();
		void Clean();
	public:
		auto begin()       { return m_layerStack.begin();  }
		auto end()         { return m_layerStack.end();    }
		auto begin() const { return m_layerStack.cbegin(); }
		auto end()   const { return m_layerStack.cend();   }
	private:
		std::vector<std::unique_ptr<Layer>> m_layerStack;
		std::vector<std::unique_ptr<Layer>> m_toPush;
		std::vector<Layer*> m_toRemove;
	};

}

