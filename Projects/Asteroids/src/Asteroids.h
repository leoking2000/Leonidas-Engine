#pragma once
#include "ECS/IComponentStore.h"
#include <LEO/ECS/EntityManager.h>


class Game 
{
public:
	Game();

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	~Game();
public:
	void UpdateGame();
	void RenderGame();
private:
	//void SpawnShip();
	//void SpawnBullet(const glm::vec2 pos, const glm::vec2 dir);
	//void SpawnAsteroid(const glm::vec2 pos, u32 size);
private:
	LEO::EntityManager m_entityManager;
	LEO::entity_id m_player_id = 0;
};


