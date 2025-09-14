#include <LEO/LeoEngine.h>
#include <Imgui/imgui.h>

// Components
struct Sphere
{
	glm::vec2 pos;
	f32 radius;
	glm::vec2 vel;
	i32 hp;
};

#define MAX_ENTITIES 200

static LEO::entity_id next_id = 0;
static std::vector<LEO::entity_id> m_free_ids;

static LEO::ComponentArray<Sphere, MAX_ENTITIES> sphere_store;
//static LEO::SparseComponentStore<Sphere> sphere_store;


static void CreateEntity(const Sphere& sphere)
{
	if (m_free_ids.empty())
	{
		sphere_store.AddComponent(next_id, sphere);
		next_id++;
	}
	else
	{
		sphere_store.AddComponent(m_free_ids.back(), sphere);
		m_free_ids.pop_back();
	}
}

static void DestroyEntity(LEO::entity_id id)
{
	sphere_store.RemoveComponent(id);
	m_free_ids.emplace_back(id);
}


static void Init()
{
	for (i32 i = 0; i < MAX_ENTITIES / 2; i++)
	{
		CreateEntity(Sphere{ {LEO::RandFloat(0.0f, 1600.0f), LEO::RandFloat(0.0f, 900.0f)}, 
			20.0f, LEO::RandDir2D(150.0f), LEO::RandInt(20, 40) });
	}
}

static void SpawnSystem()
{
	for (auto [i, sphere] : sphere_store)
	{
		if (sphere.hp <= 0) {
			DestroyEntity(i);

			if (sphere.radius <= 5.0f) continue;

			const f32 r = sphere.radius / 2.0f;
			CreateEntity(Sphere{ sphere.pos, r, LEO::RandDir2D(150.0f), 5 });
			CreateEntity(Sphere{ sphere.pos, r, LEO::RandDir2D(150.0f), 5 });
		}
	}

}

static void MoveSystem()
{
	for (auto [i, sphere] : sphere_store) 
	{
		sphere.pos += sphere.vel * LEO::DeltaTime();

		const f32 r = sphere.radius;
		const f32 winW = (f32)LEO::WinWidth();
		const f32 winH = (f32)LEO::WinHeight();
		if (sphere.pos.x - r < 0)    { sphere.pos.x = r;         sphere.vel.x *= -1; }
		if (sphere.pos.x + r > winW) { sphere.pos.x = winW - r;  sphere.vel.x *= -1; }
		if (sphere.pos.y - r < 0)    { sphere.pos.y = r;         sphere.vel.y *= -1; }
		if (sphere.pos.y + r > winH) { sphere.pos.y = winH - r;  sphere.vel.y *= -1; }
	}
}

static void CollisionSystem()
{
	for (auto itA = sphere_store.begin(); itA != sphere_store.end(); ++itA)
	{
		Sphere& a = (*itA).comp;
		for (auto itB = itA.next(); itB != sphere_store.end(); ++itB)
		{
			Sphere& b = (*itB).comp;

			glm::vec2 delta = b.pos - a.pos;
			float dist2 = glm::dot(delta, delta);
			float r = a.radius + b.radius;

			if (dist2 < r * r) // collision!
			{
				float dist = std::sqrt(dist2);
				if (dist == 0.0f) dist = 0.01f; // avoid divide by zero
				glm::vec2 normal = delta / dist;
				// Push spheres apart by half the overlap each
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


static void RenderSystem()
{
	u32 count = 0;
	for(auto [i, sphere] : sphere_store) 
	{
		LEO::Color color = sphere.radius <= 10.0f ? LEO_DARKGREEN : LEO_BLEU;
		color = sphere.radius <= 5.0f ? LEO_RED : color;

		LEO::RenderCircle(sphere.pos, sphere.radius, color);

		count += 1;
	}

	// ImGui window
	ImGui::Begin("Stress Test");
	ImGui::Text("FPS: %u", LEO::CurrentFPS());
	ImGui::Text("Sphere count: %u", count);
	ImGui::Text("Sphere next id: %u", next_id);
	ImGui::End();
}


int main(int argc, char** argv)
{
	LEO::GetDefaultLogChannel().SetLoggingLevel(LEO::LogLevel::DEBUG);

	LEO::CreateWindow(1600, 900, "Leonidas Engine", LEO::WIN_FLAG_ESC_CLOSE);
	LEO::SetClearColor(LEO_BLACK);

	LEO::RandSetSeed(1234);
	
	Init();
	sphere_store.ApplyPending();

	while (!LEO::ShouldCloseWindow())
	{
		LEO::StartFrame();

		SpawnSystem();
		MoveSystem();
		CollisionSystem();
		RenderSystem();

		sphere_store.ApplyPending();

		LEO::EndFrame();
	}


	LEO::DestroyWindow();

	return 0;
}


