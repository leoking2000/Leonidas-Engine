#pragma once
#include "LEO/Log/Log.h"

namespace LEO
{
	// we hard limit entity number to 65.536, in reality (for small games) we will have less than 1000 entities!
	using leo_size_t = u16;
	using entity_id = leo_size_t;

	/// <summary>
	/// IComponentStoreBase is used to hide IComponentStore<T>, the goal is to have a ponter to a store without knowing the T
	/// used by the EntityManager
	/// </summary>
	class IComponentStoreBase
	{
	public:
		virtual            ~IComponentStoreBase()                 = default;
	public:														  
		virtual void       RemoveComponent(entity_id id)          = 0; // Marks the (Entity id, component) mapping for removal if it exist
		virtual bool       HasComponent(entity_id id) const       = 0; // Returns true if Entity id is mapped to the component, otherwise false
	public:
		virtual leo_size_t NumOfComponents() const                = 0; // Returns the number of Entity id mapped to a component
		virtual leo_size_t MaxCapacity() const                    = 0; // The Maximum capacity, how many (Entity id, component) mapping can we store
		virtual void       ApplyPending()                         = 0; // Removes the (id, comp) mark for removal, and then adds the (id, comp) mark for addition
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
		virtual       ~IComponentStore()                          = default;
	public:
		virtual void  AddComponent(entity_id id, T component)     = 0; // Marks the (Entity id, component) mapping for addition
		virtual T*    GetComponent(entity_id id)                  = 0; // Returns a pointer to the component mapped to the Entity id, otherwise nullptr if no mapping exits
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
}