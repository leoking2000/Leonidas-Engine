#pragma once
#include "LEO/LeoEngine.h"
#include "Components.h"

static void CreateEntity(leo::EntityManager& entity_manager, const Particle& p)
{
	leo::entity_id id = entity_manager.CreateEntity();
	entity_manager.AddComponent(id, p);
}

static void DestroyEntity(leo::EntityManager& entity_manager, leo::entity_id id)
{
	entity_manager.DestroyEntity(id);
}

class SpawnSystem : public leo::ISystem
{
public:
	virtual void Update(leo::f32 dt) override
	{
		p_entityManager->ForEach<Particle>([&](leo::entity_id id, Particle& particle){
			if (particle.hp <= 0) {
				
                p_entityManager->DestroyEntity(id);

				if (particle.radius <= 5.0f) return;

				const leo::f32 r = particle.radius / 2.0f;
				//CreateEntity(*p_entityManager, Particle{ particle.pos, r, LEO::RandDir2D(150.0f), 5 });
				//CreateEntity(*p_entityManager, Particle{ particle.pos, r, LEO::RandDir2D(150.0f), 5 });
			}
		});
	}
};

class MoveSystem : public leo::ISystem
{
public:
	virtual void Update(leo::f32 dt) override
	{
		p_entityManager->ForEach<Particle>([&](leo::entity_id id, Particle& particle){
			particle.pos += particle.vel * dt;

			//const f32 r = particle.radius;
			//const f32 winW = (f32)leo::WinWidth();
			//const f32 winH = (f32)leo::WinHeight();
			//
			//if (particle.pos.x - r < 0)    { particle.pos.x = r;         particle.vel.x *= -1; }
			//if (particle.pos.x + r > winW) { particle.pos.x = winW - r;  particle.vel.x *= -1; }
			//if (particle.pos.y - r < 0)    { particle.pos.y = r;         particle.vel.y *= -1; }
			//if (particle.pos.y + r > winH) { particle.pos.y = winH - r;  particle.vel.y *= -1; }
		});
	}
};

class CollisionSystem : public leo::ISystem
{
public:
	virtual void Update(leo::f32 dt) override
	{
		auto& particle_store = *p_entityManager->GetComponentStore<Particle>();

		for (auto itA = particle_store.begin(); itA != particle_store.end(); ++itA)
		{
			Particle& a = (*itA).comp;
			for (auto itB = itA.next(); itB != particle_store.end(); ++itB)
			{
				Particle& b = (*itB).comp;

				glm::vec2 delta = b.pos - a.pos;
				float dist2 = glm::dot(delta, delta);
				float r = a.radius + b.radius;

				if (dist2 < r * r) // collision!
				{
					float dist = std::sqrt(dist2);
					if (dist == 0.0f) dist = 0.01f; // avoid divide by zero
					glm::vec2 normal = delta / dist;
					// Push Particles apart by half the overlap each
					float overlap = 0.5f * (r - dist);
					a.pos -= normal * overlap;
					b.pos += normal * overlap;
					// Reflect velocities along the collision normal
					a.vel = glm::reflect(a.vel, normal);
					b.vel = glm::reflect(b.vel, -normal);
					a.hp -= 1;
					b.hp -= 1;
				}
			}
		}
	}	

};

class RenderSystem : public leo::ISystem
{
public:
	virtual void Update(leo::f32 dt) override
	{
		p_entityManager->ForEach<Particle>([&](leo::entity_id id, Particle& Particle) {
			//leo::Color color = Particle.radius <= 10.0f ? LEO_DARKGREEN : LEO_BLEU;
			//color = Particle.radius <= 5.0f ? LEO_RED : color;
			//
			//leo::RenderCircle(Particle.pos, Particle.radius, color);
		});
	}
};