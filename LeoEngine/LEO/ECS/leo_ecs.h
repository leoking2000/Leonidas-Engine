#pragma once
#include <array>
#include <bitset>
#include <vector>
#include <memory>
#include <unordered_map>

#include "LEO/Log/Log.h"

namespace LEO
{
	// we hard limit entity number to 65.536, in reality (for small games) we will have less than 1000 entities!

	using leo_size_t = u16;
	using entity_id  = u16;

	/// <summary>
	/// IComponentStore is the interface for the container that stores the component data of a given type T 
	/// and the mapping between entity_id and the data.
	/// </summary>
	/// <typeparam name="T">A Default-contratable type that holds the data of an component</typeparam>
	template<typename T>
	class IComponentStore
	{
	public:
		virtual void       AddComponent(entity_id id, T component) = 0; // Marks the (Entity id, component) mapping for addition
		virtual void       RemoveComponent(entity_id id)           = 0; // Marks the (Entity id, component) mapping for removal if it exist
		virtual bool       HasComponent(entity_id id) const        = 0; // Returns true if Entity id is mapped to the component, otherwise false
		virtual T*         GetComponent(entity_id id)              = 0; // Returns a pointer to the component mapped to the Entity id, otherwise nullptr if no mapping exits
		virtual leo_size_t NumOfComponents() const                 = 0; // Returns the number of Entity id mapped to a component
		virtual void       ApplyPending()                          = 0; // Removes the (id, comp) mark for removal, and then adds the (id, comp) mark for addition 
	public:
		virtual ~IComponentStore() = default;
	};

	/// <summary>
	/// A IComponentStore that uses a std::array
	/// </summary>
	/// <typeparam name="T">A Default-contratable type that holds the data of an component</typeparam>
	/// <typeparam name="SIZE">the size of the std::array</typeparam>
	template<typename T, leo_size_t SIZE>
	class ComponentArray : public IComponentStore<T>
	{
	public:
		ComponentArray() { m_toAdd.reserve(SIZE); m_toRemove.reserve(SIZE); } // memory is cheap lol, ram go brrrr :D
		virtual ~ComponentArray() = default;
	public:
		virtual void AddComponent(entity_id id, T component) override
		{ 
			LEOASSERTF(id < (entity_id)m_data.size(), "Invalid ID: {}, size of ComponentArray is {}", id, m_data.size());
			LEOASSERTF(!m_exist[id], "Entity with ID: {} already has the component", id);

			if (id >= m_data.size()) { 
				LEOLOGERROR("Invalid ID: {}, size of ComponentArray is {}, we ignore this", id, m_data.size());
				return; 
			}

			if (m_exist[id]) {
				LEOLOGWARN("Entity with ID: {} already has the component, we ignore this, please just modifiy the component if you want to reset it", id);
				return;
			}

			m_toAdd.emplace_back(id, std::move(component)); 
		}

		virtual void RemoveComponent(entity_id id) override
		{ 
			LEOASSERTF(id < m_data.size(), "Invalid ID: {}, size of ComponentArray is {}", id, m_data.size());
			LEOASSERTF(m_exist[id], "Entity with ID: {} does not have the component", id);

			if (id >= m_data.size()) {
				LEOLOGERROR("Invalid ID: {}, size of ComponentArray is {}, we ignore this", id, m_data.size());
				return;
			}

			if (!m_exist[id]) {
				LEOLOGWARN("Entity with ID: {} does not have the component, we ignore this, please check first if the component exits", id);
				return;
			}

			m_toRemove.emplace_back(id); 
		}
		
		virtual bool HasComponent(entity_id id) const override
		{ 
			LEOASSERTF(id < m_data.size(), "Invalid ID: {}, size of ComponentArray is {}", id, m_data.size());

			if (id >= m_data.size()) {
				LEOLOGERROR("Invalid ID: {}, size of ComponentArray is {}, so we return false", id, m_data.size());
				return false;
			}

			return m_exist[id]; 
		}

		virtual T*   GetComponent(entity_id id) override
		{ 
			LEOASSERTF(id < m_data.size(), "Invalid ID: {}, size of ComponentArray is {}", id, m_data.size());

			if (id >= m_data.size()) {
				LEOLOGERROR("Invalid ID: {}, size of ComponentArray is {}, so we return nullptr", id, m_data.size());
				return nullptr;
			}

			return m_exist[id] ? &m_data[id] : nullptr; 
		}

		virtual leo_size_t NumOfComponents() const override { return m_count; }

		virtual void ApplyPending() override {
			// Remove pending components
			for (entity_id id : m_toRemove) {
				if (m_exist[id]) {
					m_exist[id] = false;
					std::destroy_at(&m_data[id]);
					m_count--;
				}
			}
			m_toRemove.clear();

			// Add pending components
			for (auto& [id, comp] : m_toAdd) {
				if (!m_exist[id]) {
					m_exist[id] = true;
					m_count++;	
				}
				else {
					std::destroy_at(&m_data[id]);
				}
				m_data[id] = std::move(comp);
			}
			m_toAdd.clear();
		}
	public:
		struct Item { entity_id id; T& comp; };                         // Represents the (Entity id, component) mapping

		class Iterator
		{
		public:
			Iterator(size_t index, ComponentArray* store)
				: m_index(index), m_parent(store)
			{
				skipToNextValid();
			}
		public:
			Iterator& advanceBy(leo_size_t n = 1) {
				for (leo_size_t i = 0; i < n; ++i) ++(*this);
				return *this;
			}

			Iterator next(leo_size_t n = 1)  {
				auto it = Iterator(*this);
				it.advanceBy(n);
				return it;
			}

			virtual bool reachEnd() 
			{
				return m_index == SIZE;
			}

			virtual void reset() 
			{
				m_index = 0;
				skipToNextValid();
			}

			bool operator!=(const Iterator& o) const  {
				return m_index != o.m_index || m_parent != o.m_parent;
			}

			Item operator*() const  {
				return Item{ m_index, m_parent->m_data[m_index] };
			}

			Iterator operator++(int)  {
				auto copy = Iterator(*this);
				++(*this);
				return copy;
			}

			Iterator& operator++()  {
				do { ++m_index; } while (m_index < SIZE && !m_parent->m_exist[m_index]);
				return *this;
			}

		private:
			void skipToNextValid() {
				while (m_index < SIZE && !m_parent->m_exist[m_index]) {
					++m_index;
				}
			}
		private:
			leo_size_t m_index;
			ComponentArray* m_parent;
		};

		virtual Iterator begin()  
		{ 
			leo_size_t start = 0;
			while (start < SIZE && !m_exist[start]) ++start;
			return Iterator(start, this);
		}

		virtual Iterator end()  
		{
			return Iterator(SIZE, this);
		}
	private:
		std::array<T, SIZE> m_data = {};
		std::bitset<SIZE> m_exist  = {};

		std::vector<std::pair<entity_id, T>> m_toAdd;
		std::vector<entity_id> m_toRemove;

		leo_size_t m_count = 0;
	};



}

