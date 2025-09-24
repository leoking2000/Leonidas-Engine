#pragma once


namespace LEO 
{
    class EntityManager;

    class ISystem
    {
    public:
        virtual ~ISystem() = default;
        virtual void Update(f32 dt) = 0;

        void SetEntityManager(EntityManager* entityManager) 
        { 
            p_entityManager = entityManager;
        }

    protected:
        EntityManager* p_entityManager = nullptr;
    };
}