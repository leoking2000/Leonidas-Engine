#include "SandBox.h"
#include "Components.h"
#include "Systems.h"
#include <imgui/imgui.h>


void SandBoxLayer::OnCreate()
{
	LEO::GetDefaultLogChannel().SetLoggingLevel(LEO::LogLevel::DEBUG);
	LEO::SetClearColor(LEO_BLACK);
	LEO::RandSetSeed(1234);

    m_entityManager.RegisterComponentStore<Particle>(std::make_unique<LEO::ComponentArray<Particle, 500>>());

	for (i32 i = 0; i < 300; i++)
	{
		CreateEntity(m_entityManager, Particle{ {LEO::RandFloat(0.0f, 1600.0f), LEO::RandFloat(0.0f, 900.0f)}, 
			20.0f, LEO::RandDir2D(150.0f), LEO::RandInt(20, 40) });
	}

    m_entityManager.RegisterSystem<MoveSystem>();
	m_entityManager.RegisterSystem<CollisionSystem>();
	m_entityManager.RegisterSystem<SpawnSystem>();
	m_entityManager.RegisterSystem<RenderSystem>();
}

void SandBoxLayer::OnUpdate()
{
    m_entityManager.Update(LEO::DeltaTime());

    ImGui::Begin("Stress Test");
    ImGui::Text("FPS: %u", LEO::CurrentFPS());
    ImGui::Text("Sphere count exits: %u", m_entityManager.GetComponentStore<Particle>()->NumOfComponents());
    ImGui::End();

}
