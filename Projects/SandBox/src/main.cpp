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

static LEO::entity_id next_id = 0;
static LEO::ComponentStore<Sphere> sphere_store;


void CreateRandomSphere()
{
	sphere_store.AddComponentTo(next_id, Sphere{ LEO::RandFloat2(0.0f, 900.0f), 15.0f, LEO::RandDir2D(150.0f), 
		LEO::RandInt(1, 10)});
	next_id++;
}

void Init()
{
	for (i32 i = 0; i < 500; i++)
	{
		CreateRandomSphere();
	}
}

void SpawnSystem()
{
	if (LEO::MouseButtonPressed(LEO::MOUSE_BUTTON_RIGHT))
	{
		CreateRandomSphere();
	}
}

void RenderStats()
{
	// Compute statistics
	u32 count = 0;

	for (auto& [id, s] : sphere_store)
	{
		count++;
	}

	// ImGui window
	ImGui::Begin("Stress Test");
	ImGui::Text("FPS: %u", LEO::CurrentFPS());
	ImGui::Text("Sphere count: %u", count);
	ImGui::Text("Sphere next id: %u", next_id);
	ImGui::End();
}


void MoveSystem()
{
	for (auto& [e, sphere] : sphere_store)
	{
		sphere.pos += sphere.vel * LEO::DeltaTime();

		const f32 r = sphere.radius;
		const f32 winW = (f32)LEO::WinWidth();
		const f32 winH = (f32)LEO::WinHeight();
		if (sphere.pos.x - r < 0)    { sphere.pos.x = r;         sphere.vel.x *= -1; }
		if (sphere.pos.x + r > winW) { sphere.pos.x = winW - r;  sphere.vel.x *= -1; }
		if (sphere.pos.y - r < 0)    { sphere.pos.y = r;         sphere.vel.y *= -1; }
		if (sphere.pos.y + r > winH) { sphere.pos.y = winH - r;  sphere.vel.y *= -1; }

		if (sphere.hp <= 0) {
			sphere_store.RemoveComponentFrom(e);
			//CreateRandomSphere();
		}
	}
}

void CollisionSystem()
{
	for (auto itA = sphere_store.begin(); itA != sphere_store.end(); ++itA)
	{
		Sphere& a = itA->second;
		for (auto itB = std::next(itA); itB != sphere_store.end(); ++itB)
		{
			Sphere& b = itB->second;

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

void RenderSystem()
{
	for (auto& [e, sphere] : sphere_store)
	{
		LEO::RenderCircle(sphere.pos, sphere.radius, LEO_DARKGREEN);
	}
}


int main(int argc, char** argv)
{
	LEO::GetDefaultLogChannel().SetLoggingLevel(LEO::LogLevel::DEBUG);

	LEO::CreateWindow(1600, 900, "Leonidas Engine", LEO::WIN_FLAG_ESC_CLOSE);
	LEO::SetClearColor(LEO_BLACK);
	LEO::SetFPSTarget(60);
	
	Init();

	while (!LEO::ShouldCloseWindow())
	{
		LEO::StartFrame();

		SpawnSystem();
		MoveSystem();
		CollisionSystem();
		RenderSystem();
		RenderStats();

		sphere_store.Update();

		LEO::EndFrame();
	}


	LEO::DestroyWindow();

	return 0;
}


