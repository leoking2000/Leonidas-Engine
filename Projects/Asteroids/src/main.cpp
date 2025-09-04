#include <LEO/LeoEngine.h>

#define MAX_ENTITIES 1000
#define MAX_VERTICES 32

enum EnityType {
	PLAYER = 0,
	ASTEROID = 1,
	BULLET = 2
};

struct Entity
{
	EnityType type;

	// transform
	glm::vec2 pos;
	glm::vec2 vel;
	f32       rot;

	// color
	LEO::WIN::LeoColor color;

	// Asteroid data
	float rotationSpeed;
	int vertexCount;
	glm::vec2 baseShape[MAX_VERTICES];
	float radius;
};

static u32    entity_count = 0;
static Entity entity_array[MAX_ENTITIES] = {};

 
static Entity* AddSteroid(glm::vec2 pos, f32 size, i32 vertexCount)
{
	Entity* e = &entity_array[entity_count];
	entity_count++;

	e->type = ASTEROID;

	e->pos = pos;
	e->vel = LEO::UTL::RandFloat2(-150, 150);
	e->rot = 0.0f;
	
	e->color = LEO_DARKGRAY;

	e->rotationSpeed = glm::radians(LEO::UTL::RandFloat(-180, 180));
	e->vertexCount = vertexCount;
	float step = 360.0f / vertexCount;

	e->radius = 0.0f;
	for (i32 i = 0; i < vertexCount; i++) {
		float angle = glm::radians(step * i);
		float radius = size * (0.7f + (LEO::UTL::RandFloat(0, 30) / 100.0f)); // jitter radius
		e->baseShape[i] = { glm::cos(angle) * radius, glm::sin(angle) * radius };

		if (e->radius >= radius || e->radius == 0.0f) {
			e->radius = radius;
		}
	}

	return e;
}

void ResolveAsteroidCollision(Entity& a, Entity& b) {
	glm::vec2 delta = b.pos - a.pos;
	float dist2 = glm::length2(delta);
	float minDist = a.radius + b.radius;

	if (dist2 < minDist * minDist && dist2 > 0.0f) {
		float dist = std::sqrt(dist2);
		glm::vec2 normal = delta / dist;

		// Push apart
		float overlap = minDist - dist;
		a.pos -= normal * (overlap * 0.5f);
		b.pos += normal * (overlap * 0.5f);

		// Relative velocity
		glm::vec2 relVel = b.vel - a.vel;
		float velAlongNormal = glm::dot(relVel, normal);

		if (velAlongNormal > 0) return; // already separating

		float restitution = 1.0f; // perfectly elastic
		float j = -(1.0f + restitution) * velAlongNormal / 2.0f;

		glm::vec2 impulse = normal * j;
		a.vel -= impulse;
		b.vel += impulse;
	}
}


static void MovementSystem()
{
	for (u32 i = 0; i < entity_count; i++)
	{
		Entity* e = &entity_array[i];

		e->pos = e->pos + e->vel * LEO::WIN::DeltaTime();
		e->rot += e->rotationSpeed * LEO::WIN::DeltaTime();

		// Bounce on X edges
		if (e->pos.x - e->radius < 0) {
			e->pos.x = e->radius;
			e->vel.x *= -1;
		}
		if (e->pos.x + e->radius > LEO::WIN::Width()) {
			e->pos.x = LEO::WIN::Width() - e->radius;
			e->vel.x *= -1;
		}

		// Bounce on Y edges
		if (e->pos.y - e->radius < 0) {
			e->pos.y = e->radius;
			e->vel.y *= -1;
		}
		if (e->pos.y + e->radius > LEO::WIN::Height()) {
			e->pos.y = LEO::WIN::Height() - e->radius;
			e->vel.y *= -1;
		}

		// Collisions
		for (size_t i = 0; i < entity_count; i++) {
			for (size_t j = i + 1; j < entity_count; j++) {
				ResolveAsteroidCollision(entity_array[i], entity_array[j]);
			}
		}

	}
}

static void RenderSystem()
{
	for (u32 i = 0; i < entity_count; i++)
	{
		Entity* e = &entity_array[i];

		if (e->type == ASTEROID)
		{
			LEOASSERT(e->vertexCount >= 3, "we need at least 3 vertices!!!").LEOWATCH(e->vertexCount).LEOWATCH(i);

			for (int i = 1; i < e->vertexCount - 1; i++) {
				glm::vec2 v0 = e->pos + glm::rotate(e->baseShape[0], e->rot);
				glm::vec2 v1 = e->pos + glm::rotate(e->baseShape[i], e->rot);
				glm::vec2 v2 = e->pos + glm::rotate(e->baseShape[i + 1], e->rot);

				LEO::WIN::RenderTriangle(v2, v1, v0, e->color);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////

static void GameInit()
{
	AddSteroid(LEO::WIN::MousePosition(), 70, LEO::UTL::RandInt(12, 32));
}

static void GameUpdate()
{
	MovementSystem();


	if (LEO::WIN::MouseButtonPressed(LEO::WIN::MOUSE_BUTTON_LEFT))
	{
		AddSteroid(LEO::WIN::MousePosition(), 70, LEO::UTL::RandInt(12, 32));
	}
}

static void GameRender()
{
	RenderSystem();
}


int main()
{
	LEO::WIN::CreateWindow(1600, 900, "Asteroids");
	//LEO::WIN::SetFPSTarget(60u);

	LEO::WIN::SetClearColor(LEO_BLACK);
	bool game_over = false;

	GameInit();

	while (!LEO::WIN::ShouldClose() && !game_over)
	{
		LEO::WIN::StartFrame();

		GameUpdate();
		GameRender();

		if (LEO::WIN::KeyDown(LEO::WIN::ESCAPE)) { game_over = true; }

		std::string title = std::format("Leonidas Engine FPS:{} | Entity Count: {}", LEO::WIN::CurrentFPS(), entity_count);
		LEO::WIN::SetTitle(title);

		LEO::WIN::EndFrame();
	}

	LEO::WIN::DestroyWindow();

	return 0;
}