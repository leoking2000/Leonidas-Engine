#pragma once
#include <array>
#include <bitset>
#include <vector>

#include "LEO/Log/Log.h"

namespace LEO
{
	// we hard limit entity number to 65.536, in reality (for small games) we will have less than 1000 entities!

	using leo_size_t = u16;
	using entity_id  = leo_size_t;

	/// <summary>
	/// IComponentStore is the interface for the container that stores the component data of a given type T 
	/// and the mapping between entity_id and the data.
	/// </summary>
	/// <typeparam name="T">A Default-contratable type that holds the data of an component</typeparam>
	template<typename T>
	class IComponentStore
	{
	public:
		virtual            ~IComponentStore()                      = default;
	public:
		virtual void       AddComponent(entity_id id, T component) = 0; // Marks the (Entity id, component) mapping for addition
		virtual void       RemoveComponent(entity_id id)           = 0; // Marks the (Entity id, component) mapping for removal if it exist
		virtual bool       HasComponent(entity_id id) const        = 0; // Returns true if Entity id is mapped to the component, otherwise false
		virtual T*         GetComponent(entity_id id)              = 0; // Returns a pointer to the component mapped to the Entity id, otherwise nullptr if no mapping exits
		virtual void       ApplyPending()                          = 0; // Removes the (id, comp) mark for removal, and then adds the (id, comp) mark for addition 
	public:
		virtual leo_size_t NumOfComponents() const                 = 0; // Returns the number of Entity id mapped to a component
		virtual leo_size_t MaxCapacity() const                     = 0; // The Maximum capacity, how many (Entity id, component) mapping can we store
	protected:
		/// <summary>
		/// Returns the index of the first valid (existing) entity at or after `from`.
		//  Returns MaxCapacity() if none are valid. Used internally by the iterator.
		/// </summary>
		virtual entity_id FindNextValidIndex(entity_id from) const = 0;
	public:
		struct Item { entity_id id; T& comp; };

		// a forward iterator for the (Entity id, component) mapping
		class ComponentIterator
		{
		public:
			ComponentIterator(entity_id id, IComponentStore* store) 
				: m_current_id(id), m_store(store)
			{
				m_current_id = m_store->FindNextValidIndex(m_current_id);
			}
		public:
			ComponentIterator& operator++() {
				// we want the next Valid index after the current one
				m_current_id = m_store->FindNextValidIndex(m_current_id + 1);
				return *this;
			}

			ComponentIterator operator++(int) {
				auto copy = ComponentIterator(*this);
				++(*this);
				return copy;
			}
			
			// NOTE: Returned Item T& comp remains valid until the component is removed or replaced during ApplyPending().
			Item operator*() const {
				return Item{ m_current_id, *m_store->GetComponent(m_current_id) };
			}

			bool operator!=(const ComponentIterator& o) const {
				return m_current_id != o.m_current_id;
			}
		public:
			ComponentIterator next() const {
				auto it = ComponentIterator(*this);
				++it;
				return it;
			}
		private:
			IComponentStore* m_store;
			entity_id m_current_id;
		};
	public:
		ComponentIterator begin() { return ComponentIterator(0, this); }
		ComponentIterator end()   { return ComponentIterator(MaxCapacity(), this); }
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
		// Marks the (Entity id, component) mapping for addition
		virtual void AddComponent(entity_id id, T component) override
		{ 
			if (id >= m_data.size()) { 
				LEOLOGERROR("Invalid ID: {}, size of ComponentArray is {}, we ignore this", id, m_data.size());
				return; 
			}

			// either the entity does not have the component, or is mark for removal
			// todo: use a std::bitset<SIZE> m_toRemoveMask
			if (m_exist[id] && std::find(m_toRemove.begin(), m_toRemove.end(), id) == m_toRemove.end()) {
				LEOLOGWARN("Entity with ID: {} already has the component, we ignore this, please just modifiy the component if you want to reset it", id);
				return;
			}

			m_toAdd.emplace_back(id, std::move(component)); 
		}

		// Marks the (Entity id, component) mapping for removal if it exist
		virtual void RemoveComponent(entity_id id) override
		{ 
			if (id >= m_data.size()) {
				LEOLOGERROR("Invalid ID: {}, size of ComponentArray is {}, we ignore this", id, m_data.size());
				return;
			}

			if (!m_exist[id]) {
				LEOLOGWARN("Entity with ID: {} does not have the component, we ignore this, please check first if the component exits", id);
				return;
			}

			// todo: use a std::bitset<SIZE> m_toRemoveMask
			if (std::find(m_toRemove.begin(), m_toRemove.end(), id) == m_toRemove.end())
			{
				m_toRemove.emplace_back(id);
			}		
		}
		
		// Returns true if Entity id is mapped to the component, otherwise false
		virtual bool HasComponent(entity_id id) const override
		{ 
			if (id >= m_data.size()) {
				LEOLOGERROR("Invalid ID: {}, size of ComponentArray is {}, so we return false", id, m_data.size());
				return false;
			}

			return m_exist[id]; 
		}

		// Returns a pointer to the component mapped to the Entity id, otherwise nullptr if no mapping exits
		virtual T*   GetComponent(entity_id id) override
		{ 
			if (id >= m_data.size()) {
				LEOLOGERROR("Invalid ID: {}, size of ComponentArray is {}, so we return nullptr", id, m_data.size());
				return nullptr;
			}

			return m_exist[id] ? &m_data[id] : nullptr; 
		}

		// Returns the number of Entity id mapped to a component
		virtual leo_size_t NumOfComponents() const override { return m_count; }

		// The Maximum capacity, how many (Entity id, component) mapping can we store
		virtual leo_size_t MaxCapacity() const override { return SIZE; }

		// Removes the(id, comp) mark for removal, and then adds the(id, comp) mark for addition
		virtual void ApplyPending() override {
			// Remove pending components
			for (entity_id id : m_toRemove) {
				if (m_exist[id]) {
					m_exist[id] = false;
					m_data[id] = T{};
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
				m_data[id] = std::move(comp);
			}
			m_toAdd.clear();
		}
	protected:
		/// <summary>
		/// Returns the index of the first valid (existing) entity at or after `from`.
		//  Returns MaxCapacity() if none are valid. Used internally by the iterator.
		/// </summary>
		virtual entity_id FindNextValidIndex(entity_id from) const override
		{
			entity_id index = from;
			while (index < SIZE && !m_exist[index]) {
				++index;
			}
			return index;
		}
	private:
		std::array<T, SIZE> m_data = {};
		std::bitset<SIZE> m_exist  = {};

		std::vector<std::pair<entity_id, T>> m_toAdd;
		std::vector<entity_id> m_toRemove;

		leo_size_t m_count = 0;
	};
}

