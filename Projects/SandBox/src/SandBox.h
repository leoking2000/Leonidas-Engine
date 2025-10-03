#pragma once
#include "LEO/LeoEngine.h"


class SandBoxLayer : public LEO::Layer
{
public:
    virtual void OnCreate() override;    
    virtual void OnUpdate() override;
private:
    LEO::EntityManager m_entityManager;
};


