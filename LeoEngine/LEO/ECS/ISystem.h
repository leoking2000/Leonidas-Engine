#pragma once

namespace LEO 
{
    class EntityManager;

    class ISystem
    {
    public:
        virtual void Init() {}
        virtual void Update(f32 dt) = 0;
        virtual void Render() {}
    public:
        void SetEntityManager(EntityManager* entityManager) 
        { 
            p_entityManager = entityManager;
        }
    public:
        virtual ~ISystem() = default;
    protected:
        EntityManager* p_entityManager = nullptr;
    };
}