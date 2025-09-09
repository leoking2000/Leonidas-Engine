#pragma once
#include <unordered_map>
#include <vector>

namespace LEO
{
    typedef u32 entity_id;

    /**
     * ComponentStore<T>
     * -----------------
     * Manages components of type T, keyed by entity_id.
     */
    template<typename T>
    class ComponentStore
    {
    public:
        // Marks the component for addition. Actual addition happens when `Update()` is called.
        void AddComponentTo(entity_id id, T component)
        {
            m_toAdd.emplace_back(id, component);
        }

        /// Marks the component for removal. Actual deletion happens when `Update()` is called.
        void RemoveComponentFrom(entity_id id)
        {
            m_toRemove.emplace_back(id);
        }

        /// Does the entity have this component?
        bool HasComponent(entity_id id) const
        {
            return m_mapping.find(id) != m_mapping.end();
        }

        /// Get a pointer to the component, or nullptr if not found.
        T* GetComponent(entity_id id)
        {
            auto it = m_mapping.find(id);
            return (it != m_mapping.end()) ? &it->second : nullptr;
        }

        const T* GetComponent(entity_id id) const
        {
            auto it = m_mapping.find(id);
            return (it != m_mapping.end()) ? &it->second : nullptr;
        }

        // Flush queued additions and removals
        void Update()
        {
            for (entity_id id : m_toRemove)
            {
                m_mapping.erase(id);
            }
            m_toRemove.clear();

            for (auto& [id, comp] : m_toAdd)
            {
                m_mapping[id] = std::move(comp);
            }
            m_toAdd.clear();
        }
    public:
        /// Iteration support (range-based for loops).
        auto begin() { return m_mapping.begin(); }
        auto end() { return m_mapping.end(); }
        auto begin() const { return m_mapping.begin(); }
        auto end()   const { return m_mapping.end(); }
    public:
        u64 EstimatedMemoryUsage() const
        {
            u64 memory = 0;

            // unordered_map
            memory += sizeof(m_mapping);
            memory += m_mapping.size() * (sizeof(entity_id) + sizeof(T) + sizeof(void*) * 2);

            // deferred additions
            memory += sizeof(m_toAdd) + m_toAdd.capacity() * sizeof(std::pair<entity_id, T>);

            // deferred removals
            memory += sizeof(m_toRemove) + m_toRemove.capacity() * sizeof(entity_id);

            return memory;
        }
    private:
        std::unordered_map<entity_id, T> m_mapping;
        std::vector<std::pair<entity_id, T>> m_toAdd;
        std::vector<entity_id> m_toRemove;
    };

#if 0
    // Abstract System
    class System {
    public:
        virtual ~System() = default;
        virtual void Update(float dt) = 0;
    };

    class Entity; // forward declaration

    class EntityManager
    {
    public:
        Entity CreateEntity();

        void DestroyEntity(entity_id id)
        {
            if (m_aliveEntities.erase(id)) {
                // TODO: call RemoveEntity(id) on all component stores if needed
                m_freeIds.push(id);
            }
        }

        template<typename T>
        ComponentStore<T>& GetComponentStore() {
            auto type = std::type_index(typeid(T));
            if (m_componentStores.find(type) == m_componentStores.end()) {
                m_componentStores[type] = std::make_shared<ComponentStore<T>>();
            }
            return *std::static_pointer_cast<ComponentStore<T>>(m_componentStores[type]);
        }

        void AddSystem(std::unique_ptr<System> system)
        {
            m_systems.emplace_back(std::move(system));
        }

        void Update(float dt)
        {
            for (auto& sys : m_systems)
            {
                sys->Update(dt);
            }
        }

        bool IsAlive(entity_id id) const {
            return m_aliveEntities.find(id) != m_aliveEntities.end();
        }

    private:
        entity_id m_nextEntityId = 0;
        std::queue<entity_id> m_freeIds;
        std::unordered_set<entity_id> m_aliveEntities;

        std::unordered_map<std::type_index, std::shared_ptr<void>> m_componentStores;
        std::vector<std::unique_ptr<System>> m_systems;
    };


    // Entity handle
    class Entity
    {
    public:
        template<typename T>
        void AddComponent(const T& c) { manager->GetComponentStore<T>().AddComponentTo(id, c); }

        template<typename T>
        void RemoveComponent() { manager->GetComponentStore<T>().RemoveComponentFrom(id); }

        template<typename T>
        bool HasComponent() const { return manager->GetComponentStore<T>().HasComponent(id); }

        template<typename T>
        T& GetComponent() { return manager->GetComponentStore<T>().GetComponent(id); }

        template<typename T>
        const T& GetComponent() const { return manager->GetComponentStore<T>().GetComponent(id); }

        entity_id GetId() const { return id; }
        bool IsAlive() const { return manager->IsAlive(id); }

    private:
        friend class EntityManager; // only EntityManager can construct

        Entity(entity_id id_, EntityManager* m) : id(id_), manager(m) {}

        entity_id id;
        EntityManager* manager;
    };
#endif 
}
