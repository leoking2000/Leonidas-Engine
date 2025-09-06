#include "components.h"
#include "Asteroids.h"

Game::Game()
{
	m_asteroid_transform.reserve(100);
	m_asteroid_polygons.reserve(100);
}

Game::~Game()
{

}

void Game::UpdateGame()
{
	// movement
	for (u32 i = 0; i < (u32)m_asteroid_transform.size(); i++)
	{
		Transform& transfrom = m_asteroid_transform[i];
		Polygon& polygon     = m_asteroid_polygons[i];

		transfrom.position += transfrom.velocity      * LEO::DeltaTime();
		transfrom.rotation += transfrom.rotationSpeed * LEO::DeltaTime();

		// Bounce on X edges
		if (transfrom.position.x - polygon.approximentRadius < 0) {
			transfrom.position.x = polygon.approximentRadius;
			transfrom.velocity.x *= -1;
		}
		if (transfrom.position.x + polygon.approximentRadius > LEO::WinWidth()) {
			transfrom.position.x = LEO::WinWidth() - polygon.approximentRadius;
			transfrom.velocity.x *= -1;
		}

		// Bounce on Y edges
		if (transfrom.position.y - polygon.approximentRadius < 0) {
			transfrom.position.y = polygon.approximentRadius;
			transfrom.velocity.y *= -1;
		}
		if (transfrom.position.y + polygon.approximentRadius > LEO::WinHeight()) {
			transfrom.position.y = LEO::WinHeight() - polygon.approximentRadius;
			transfrom.velocity.y *= -1;
		}
	}

	// Collisions
	for (size_t i = 0; i < (u32)m_asteroid_transform.size(); i++) {
		for (size_t j = i + 1; j < (u32)m_asteroid_transform.size(); j++) {
			ResolveCollisionPolygons(m_asteroid_transform[i], m_asteroid_polygons[i], 
				m_asteroid_transform[j], m_asteroid_polygons[j]);
		}
	}

	if (LEO::MouseButtonPressed(LEO::MOUSE_BUTTON_LEFT))
	{
		AddAsteroid(LEO::MousePosition());
	}
}

void Game::RenderGame()
{
	for (u32 i = 0; i < (u32)m_asteroid_transform.size(); i++)
	{
		Transform& transfrom = m_asteroid_transform[i];
		Polygon& polygon = m_asteroid_polygons[i];

		RenderPolygon(transfrom, polygon, LEO_DARKGRAY);
	}
}

void Game::AddAsteroid(glm::vec2 pos)
{
	m_asteroid_transform.emplace_back(Transform{
		pos, 0.0f, LEO::RandDir2D(LEO::RandFloat(150.0f, 250.0f)), 
		(LEO::RandInt(0, 1) == 0 ? 1 : -1) * glm::radians(LEO::RandFloat(90.0f, 180.0f)) });

	f32 size = LEO::RandFloat(40.0f, 60.0f);
	m_asteroid_polygons.emplace_back(GenerateRendomPolygon(16, size, size + 30.0f));
}

void Game::RemoveAsteroid(u32 index)
{
	LEOASSERT(index < (u32)m_asteroid_transform.size(), "index is invalid!!");

	// move last element into position i
	m_asteroid_transform[index] = m_asteroid_transform.back();
	m_asteroid_polygons[index]  = m_asteroid_polygons.back();

	// remove the last element
	m_asteroid_transform.pop_back();
	m_asteroid_polygons.pop_back();
}
