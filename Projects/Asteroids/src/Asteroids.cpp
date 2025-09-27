#include "Asteroids.h"
#include "Window/Window.h"
#include "components.h"

#define PLAYER_SIZE 20.0f
#define PLAYER_ACCEL 2.0f
#define PLAYER_ROT_SPEED glm::radians(60.0f)
#define PLAYER_MAX_SPEED 150.0f

#define MAX_ENTITIES 500

Game::Game()
{
	m_entityManager.RegisterDenseStore<Transform, MAX_ENTITIES>();
	m_entityManager.RegisterDenseStore<Velocity, MAX_ENTITIES>();

	m_entityManager.RegisterSparseStore<Input>();
	m_entityManager.RegisterSparseStore<Ship>();

	m_entityManager.RegisterDenseStore<LifeTime, MAX_ENTITIES>();
	m_entityManager.RegisterDenseStore<Sphere, MAX_ENTITIES>();
	m_entityManager.RegisterDenseStore<HitPoints, MAX_ENTITIES>();
	m_entityManager.RegisterDenseStore<Polygon, MAX_ENTITIES>();

	m_player_id = m_entityManager.CreateEntity();
	m_entityManager.Update(0.0f);

	Polygon player_poly;
	player_poly.vertexCount = 3;
	player_poly.baseShape[0] = glm::vec2(PLAYER_SIZE * 2.0f, 0.0f);
	player_poly.baseShape[1] = glm::vec2(-PLAYER_SIZE      , PLAYER_SIZE);
	player_poly.baseShape[2] = glm::vec2(-PLAYER_SIZE      , -PLAYER_SIZE);
	player_poly.approximateRadius = PLAYER_SIZE;

	m_entityManager.AddComponent<Transform>(m_player_id, {{800, 300}, 0.0f});
	m_entityManager.AddComponent<Polygon>(m_player_id, player_poly);
	m_entityManager.Update(0.0f);
}

Game::~Game()
{

}

void Game::UpdateGame()
{
	// Build current input mask
	u8 mask = 0;
	if (LEO::KeyDown(LEO::KEY_W))     mask |= Input::Forward;
	if (LEO::KeyDown(LEO::KEY_D))     mask |= Input::RightRotation;
	if (LEO::KeyDown(LEO::KEY_A))     mask |= Input::LeftRotation;
	if (LEO::KeyDown(LEO::KEY_SPACE)) mask |= Input::Shoot;
	Input input = {mask};

	Transform& player_t = *m_entityManager.GetComponent<Transform>(m_player_id);
	ApplyMovementInput(input, player_t, 150.0f, 1.5f, LEO::DeltaTime());

	m_entityManager.Update(LEO::DeltaTime());
}

void Game::RenderGame()
{
	m_entityManager.ForEach<Polygon>([&](LEO::entity_id id, Polygon& p)
	{
		if(Transform* t = m_entityManager.GetComponent<Transform>(id))
		{
			RenderPolygon(*t, p, LEO_RED);
		}
	});
}


