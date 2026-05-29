#pragma once
#include <LEO/Utilities/LeoTypes.h>

struct Particle
{
	glm::vec2 pos   = glm::vec2(0.0);
	leo::f32 radius = 1.0f;
	glm::vec2 vel   = glm::vec2(0.0f);
	leo::i32 hp     = 1;
};

