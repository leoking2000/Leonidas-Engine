#include "components.h"
#include "Asteroids.h"

#define PLAYER_SIZE 20.0f
#define PLAYER_ACCEL 2.0f
#define PLAYER_ROT_SPEED glm::radians(60.0f)
#define PLAYER_MAX_SPEED 150.0f

Game::Game()
{
	m_transform_array.reserve(100);
	m_polygons_array.reserve(100);

	// Create the Player
	m_transform_array.emplace_back();
	m_transform_array.back().position = glm::vec2(LEO::WinWidth() / 2.0f, LEO::WinHeight() / 2.0f);
	Polygon player_poly;
	player_poly.vertexCount = 3;
	player_poly.baseShape[0] = glm::vec2(0.0f, -PLAYER_SIZE * 2.0f);
	player_poly.baseShape[1] = glm::vec2(PLAYER_SIZE, PLAYER_SIZE);
	player_poly.baseShape[2] = glm::vec2(-PLAYER_SIZE, PLAYER_SIZE);
	player_poly.approximateRadius = PLAYER_SIZE;
	m_polygons_array.emplace_back(player_poly);
}

Game::~Game()
{

}

void Game::UpdateGame()
{
	// --- Input ---
	m_input.inputs.x = LEO::KeyDown(LEO::KEY_W) || LEO::KeyDown(LEO::KEY_UP);    // forward
	m_input.inputs.y = LEO::KeyDown(LEO::KEY_S) || LEO::KeyDown(LEO::KEY_DOWN);  // backward
	m_input.inputs.z = LEO::KeyDown(LEO::KEY_D) || LEO::KeyDown(LEO::KEY_RIGHT); // right rotation
	m_input.inputs.a = LEO::KeyDown(LEO::KEY_A) || LEO::KeyDown(LEO::KEY_LEFT);  // left rotation

	// Player (index 0 assumed to be the player)
	ApplyInput(m_input, m_transform_array[0], PLAYER_ACCEL, PLAYER_ROT_SPEED, PLAYER_MAX_SPEED);


	// --- Movement ---
	const f32 dt = LEO::DeltaTime();
	const f32 winW = (f32)LEO::WinWidth();
	const f32 winH = (f32)LEO::WinHeight();
	u32 entityCount = (u32)m_transform_array.size();

	for (u32 i = 0; i < entityCount; i++)
	{
		Transform& t = m_transform_array[i];
		Polygon& p = m_polygons_array[i];

		UpdateTransform(t, dt);
		BounceOffEdges(t, p, winW, winH);
	}

	// --- Collisions ---
	for (u32 i = 0; i < entityCount; i++) {
		for (u32 j = i + 1; j < entityCount; j++) {
			ResolveCollisionPolygons(m_transform_array[i], m_polygons_array[i],
				m_transform_array[j], m_polygons_array[j]);
		}
	}

	// --- Spawning ---
	if (LEO::MouseButtonPressed(LEO::MOUSE_BUTTON_LEFT)) {
		AddAsteroid(LEO::MousePosition());
	}
}

void Game::RenderGame()
{
	for (u32 i = 0; i < (u32)m_transform_array.size(); i++)
	{
		Transform transfrom = m_transform_array[i];
		Polygon& polygon = m_polygons_array[i];

		//transfrom.position += glm::vec2(LEO::WinWidth() / 2.0f, LEO::WinHeight() / 2.0f);

		RenderPolygon(transfrom, polygon, i==0 ? LEO_DARKRED : LEO_DARKGRAY); // make the player(index 0), red
	}
}

void Game::AddAsteroid(glm::vec2 pos)
{
	m_transform_array.emplace_back(Transform{
		pos, 0.0f, LEO::RandDir2D(LEO::RandFloat(150.0f, 250.0f)), 
		(LEO::RandInt(0, 1) == 0 ? 1 : -1) * glm::radians(LEO::RandFloat(90.0f, 180.0f)) });

	f32 size = LEO::RandFloat(40.0f, 60.0f);
	m_polygons_array.emplace_back(GenerateRendomPolygon(16, size, size + 30.0f));
}

void Game::RemoveAsteroid(u32 index)
{
	LEOASSERT(index < (u32)m_transform_array.size(), "index is invalid!!");

	// move last element into position i
	m_transform_array[index] = m_transform_array.back();
	m_polygons_array[index]  = m_polygons_array.back();

	// remove the last element
	m_transform_array.pop_back();
	m_polygons_array.pop_back();
}
