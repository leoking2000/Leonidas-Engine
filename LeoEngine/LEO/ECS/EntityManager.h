#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>
#include <algorithm>

#include "IComponentStore.h"


namespace LEO
{
	class EntityManager final
	{
	public:
		EntityManager() = default;
	public:
		entity_id CreateEntity()
		{
			if (m_freeIds.empty()) {
				return m_nextId++;
			}

			entity_id id = m_freeIds.back();
			m_freeIds.pop_back();
			return id;
		}

		bool IsEntityAlive(entity_id id)
		{
			auto it = std::find(m_freeIds.begin(), m_freeIds.end(), id);
			if (it != m_freeIds.end()) {
				return false; // if id exits in m_freeIds the entity is dead
			}

			return id < m_nextId;
		}

		void DestroyEntity(entity_id id)
		{
			for (auto& [i, store] : m_componentStores)
			{
				if (store->HasComponent(id)) {
					store->RemoveComponent(id);
				}
			}
			m_freeIds.emplace_back(id);
		}
	public:
		template<typename T>
		EntityManager& AddComponent(entity_id id, T component)
		{
			IComponentStore<T>* store = GetComponentStore<T>();
			LEOASSERT(store != nullptr, "Component store has not been registered.");

			store->AddComponent(id, std::move(component));
			return *this;
		}

		template<typename T>
		void RemoveComponent(entity_id id)
		{
			IComponentStore<T>* store = GetComponentStore<T>();
			LEOASSERT(store != nullptr, "Component store has not been registered.");

			store->RemoveComponent(id);
		}

		template<typename T>
		T* GetComponent(entity_id id)
		{
			IComponentStore<T>* store = GetComponentStore<T>();
			LEOASSERT(store != nullptr, "Component store has not been registered.");

			return store->GetComponent(id);
		}

		template<typename T>
		bool HasComponent(entity_id id) const
		{
			IComponentStore<T>* store = GetComponentStore<T>();
			LEOASSERT(store != nullptr, "Component store has not been registered.");

			return store->HasComponent(id);
		}
	public:
		template<typename T>
		void RegisterComponentStore(std::unique_ptr<IComponentStore<T>> store)
		{
			const std::type_index typeId = typeid(T);
			m_componentStores[typeId] = std::move(store);
		}

		template<typename T>
		IComponentStore<T>* GetComponentStore() const
		{
			const std::type_index typeId = typeid(T);
			auto it = m_componentStores.find(typeId);
			if (it != m_componentStores.end()) {
				return static_cast<IComponentStore<T>*>(it->second.get());
			}
			return nullptr;
		}
	public:
		void Update()
		{
			for (auto& [_, store] : m_componentStores)
			{
				store->ApplyPending();
			}
		}

		template<typename T, typename Func>
		void ForEach(Func&& update)
		{
			IComponentStore<T>* store = GetComponentStore<T>();
			LEOASSERT(store != nullptr, "Component store has not been registered.");

			for (auto [id, comp] : *store)
			{
				update(id, comp);
			}
		}
	private:
		entity_id m_nextId = 0;
		std::vector<entity_id> m_freeIds; // maybe use a std::unordered_set<entity_id> (check which is faster)

		std::unordered_map<std::type_index, std::unique_ptr<IComponentStoreBase>> m_componentStores;
	};


}