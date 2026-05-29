#pragma once
#include "LEO/LeoEngine.h"


class SandBoxLayer : public leo::Layer
{
public:
    virtual void OnCreate() override;    
    virtual void OnUpdate(leo::f32) override;
private:
    leo::EntityManager m_entityManager;
};


