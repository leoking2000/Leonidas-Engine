#pragma once
#include <unordered_map>
#include <algorithm>
#include <limits>

#include "IComponentStore.h"

namespace LEO
{
	/// <summary>
	/// A IComponentStore that uses a std::unordered_map
	/// </summary>
	/// <typeparam name="T">A Default-contratable type that holds the data of an component</typeparam>
	template<typename T>
	class ComponentStoreSparse : public IComponentStore<T>
	{
	public:
		ComponentStoreSparse() = default;
		virtual ~ComponentStoreSparse() override = default;
	public:
		// Marks the (Entity id, component) mapping for addition
		virtual void AddComponent(entity_id id, T component) override
		{
			if (HasComponent(id) && std::find(m_toRemove.begin(), m_toRemove.end(), id) == m_toRemove.end()) {
				LEOLOGWARN("Entity {} already has component, ignoring AddComponent.", id);
				return;
			}

			m_toAdd.emplace_back(id, std::move(component));
		}

		// Marks the (Entity id, component) mapping for removal if it exists
		virtual void RemoveComponent(entity_id id) override
		{
			if (!HasComponent(id)) {
				LEOLOGWARN("Entity {} does not have component, ignoring RemoveComponent.", id);
				return;
			}

			m_toRemove.emplace_back(id);
		}

		// Returns true if Entity id is mapped to the component, otherwise false
		virtual bool HasComponent(entity_id id) const override
		{
			return m_data.count(id) > 0u;
		}

		// Returns a pointer to the component mapped to the Entity id, otherwise nullptr if no mapping exists
		virtual T* GetComponent(entity_id id) override
		{
			auto it = m_data.find(id);
			return it != m_data.end() ? &it->second : nullptr;
		}

		// Returns the number of Entity id mapped to a component
		virtual leo_size_t NumOfComponents() const override
		{
			return static_cast<leo_size_t>(m_data.size());
		}

		// The Maximum capacity — conceptually "unbounded" here
		virtual leo_size_t MaxCapacity() const override
		{
			return std::numeric_limits<leo_size_t>::max();
		}

		// Apply pending adds/removes
		virtual void ApplyPending() override
		{
			// Remove pending components
			for (auto id : m_toRemove)
			{
				m_data.erase(id);
			}
			m_toRemove.clear();

			// Add pending components
			for (auto& [id, comp] : m_toAdd) {
				m_data[id] = std::move(comp);
			}
			m_toAdd.clear();

			// Refresh index cache for iteration
			m_indexCache.clear();
			m_indexCache.reserve(m_data.size());
			for (auto& [id, _] : m_data)
			{
				m_indexCache.push_back(id);
			}

			std::sort(m_indexCache.begin(), m_indexCache.end());
		}
	protected:
		/// <summary>
		/// Returns the index of the first valid (existing) entity at or after `from`.
		//  Returns MaxCapacity() if none are valid. Used internally by the iterator.
		/// </summary>
		virtual entity_id FindNextValidIndex(entity_id from) const override
		{
			auto it = std::lower_bound(m_indexCache.begin(), m_indexCache.end(), from);
			if (it == m_indexCache.end())
				return MaxCapacity();
			return *it;
		}
	private:
		std::unordered_map<entity_id, T> m_data;
		std::vector<entity_id> m_indexCache; // used by iterator for deterministic iteration

		std::vector<std::pair<entity_id, T>> m_toAdd;
		std::vector<entity_id> m_toRemove;
	};
}
