#pragma once
#include <LEO/Utilities/LeoTypes.h>

namespace leo
{
    class EntityManager;

    class ISystem
    {
    public:
        virtual void Update(f32 dt) = 0;
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