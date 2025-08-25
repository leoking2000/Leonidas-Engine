#include <random>
#include "LeoRand.h"

namespace LEO::UTL
{
	static std::mt19937 rng{ std::random_device{}() }; // Default seeded with random_device

	void RandSetSeed(u32 seed)
	{
		rng.seed(seed);
	}

	i32 RandInt(i32 min, i32 max)
	{
		std::uniform_int_distribution<i32> dist(min, max);
		return dist(rng);
	}

	u32 RandUInt(u32 min, u32 max)
	{
		std::uniform_int_distribution<u32> dist(min, max);
		return dist(rng);
	}

	f32 RandFloat(f32 min, f32 max)
	{
		std::uniform_real_distribution<f32> dist(min, max);
		return dist(rng);
	}

	glm::vec2 RandFloat2(f32 min, f32 max)
	{
		std::uniform_real_distribution<f32> dist(min, max);
		return glm::vec2(dist(rng), dist(rng));
	}

	glm::vec3 RandFloat3(f32 min, f32 max)
	{
		std::uniform_real_distribution<f32> dist(min, max);
		return glm::vec3(dist(rng), dist(rng), dist(rng));
	}

	glm::vec2 RandDir2D(f32 length)
	{
		return glm::circularRand(length);
	}

	glm::vec3 RandDir3D(f32 lenght)
	{
		return glm::sphericalRand(lenght);
	}
}