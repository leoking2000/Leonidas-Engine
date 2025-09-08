#pragma once
#include <vector>
#include "components.h"

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
	void AddAsteroid(glm::vec2 pos);
	void RemoveAsteroid(u32 index);
private:
	std::vector<Transform> m_transform_array;
	std::vector<Polygon> m_polygons_array;
	Input m_input = {};
};


