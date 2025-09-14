#pragma once
#include <array>
#include <bitset>
#include <vector>

#include <unordered_map>
#include <algorithm>
#include <limits>

#include <memory>
#include <typeindex>

#include "LEO/Log/Log.h"

namespace LEO
{
	// we hard limit entity number to 65.536, in reality (for small games) we will have less than 1000 entities!

	using leo_size_t = u16;
	using entity_id  = leo_size_t;

	class IComponentStoreBase 
	{
	public:
		virtual void       RemoveComponent(entity_id id)         = 0; // Marks the (Entity id, component) mapping for removal if it exist
		virtual bool       HasComponent(entity_id id) const      = 0; // Returns true if Entity id is mapped to the component, otherwise false
		virtual leo_size_t NumOfComponents() const               = 0; // Returns the number of Entity id mapped to a component
		virtual leo_size_t MaxCapacity() const                   = 0; // The Maximum capacity, how many (Entity id, component) mapping can we store
		virtual void       ApplyPending()                        = 0; // Removes the (id, comp) mark for removal, and then adds the (id, comp) mark for addition 
		virtual            ~IComponentStoreBase()                = default;
	};

	/// <summary>
	/// IComponentStore is the interface for the container that stores the component data of a given type T 
	/// and the mapping between entity_id and the data.
	/// </summary>
	/// <typeparam name="T">A Default-contratable type that holds the data of an component</typeparam>
	template<typename T>
	class IComponentStore : public IComponentStoreBase
	{
	public:
		virtual            ~IComponentStore()                      = default;
	public:
		virtual void       AddComponent(entity_id id, T component) = 0; // Marks the (Entity id, component) mapping for addition
		virtual T*         GetComponent(entity_id id)              = 0; // Returns a pointer to the component mapped to the Entity id, otherwise nullptr if no mapping exits
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
		virtual ~ComponentArray() override = default;
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


	template<typename T>
	class SparseComponentStore : public IComponentStore<T>
	{
	public:
		SparseComponentStore()                   = default;
		virtual ~SparseComponentStore() override = default;
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


	class ISystem
	{
	public:
		virtual ~ISystem()          = default;
		virtual void Update(f32 dt) = 0;
	};

	class EntityManager final
	{
	public:
		EntityManager() = default;
	public:
		// ===== System lifecycle =====

		void RegisterSystem(std::unique_ptr<ISystem> system)
		{
			m_systems.emplace_back(std::move(system));
		}
	public:
		// ===== Entity lifecycle =====

		entity_id CreateEntity()
		{
			if (m_freeIDs.empty())
			{
				return m_nextId++;
			}
			entity_id id = m_freeIDs.back();
			m_freeIDs.pop_back();
			return id;
		}

		void DestroyEntity(entity_id id)
		{
			// Remove entity from all component stores
			for (auto& [typeId, store] : m_componentStores)
			{
				if (store->HasComponent(id))
				{
					store->RemoveComponent(id);
				}		
			}

			// Mark ID as free again
			m_freeIDs.push_back(id);
		}
	public:
		// ===== Component store methods =====

		template<typename T>
		void RegisterStore(std::unique_ptr<IComponentStore<T>> store)
		{
			const std::type_index typeId = typeid(T);
			m_componentStores[typeId] = std::move(store);
		}

		template<typename T>
		IComponentStore<T>* GetStore()
		{
			const std::type_index typeId = typeid(T);
			auto it = m_componentStores.find(typeId);
			if (it == m_componentStores.end())
			{
				return nullptr;
			}
	
			return static_cast<IComponentStore<T>*>(it->second.get());
		}
	public:
		// ===== Component store methods =====

		template<typename T>
		void AddComponent(entity_id id, T component)
		{
			if (auto* store = GetStore<T>()) {
				store->AddComponent(id, std::move(component));
			}				
		}

		template<typename T>
		void RemoveComponent(entity_id id)
		{
			if (auto* store = GetStore<T>()) {
				if (store->HasComponent(id)){
					store->RemoveComponent(id);
				}		
			}	
		}

		template<typename T>
		bool HasComponent(entity_id id) const
		{
			if (auto* store = GetStore<T>()) {
				return store->HasComponent(id);
			}
			return false;
		}

		template<typename T>
		T* GetComponent(entity_id id)
		{
			if (auto* store = GetStore<T>())
			{
				return store->GetComponent(id);
			}
			return nullptr;
		}
	public:
		void Update(f32 dt)
		{
			for (auto& system : m_systems) 
			{
				system->Update(dt);
			}

			for (auto& [_, store] : m_componentStores)
			{
				store->ApplyPending();
			}			
		}
	private:
		entity_id m_nextId = 0;
		std::vector<entity_id> m_freeIDs;

		std::vector<std::unique_ptr<ISystem>> m_systems;
		std::unordered_map<std::type_index, std::unique_ptr<IComponentStoreBase>> m_componentStores;
	};



}

