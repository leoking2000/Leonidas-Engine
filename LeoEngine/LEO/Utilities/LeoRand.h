#pragma once

namespace LEO
{
	// Sets the seed for reproducible random number generation
	void RandSetSeed(u32 seed);

	// Returns a random signed int in the range [min, max]
	i32 RandInt(i32 min, i32 max);

	// Returns a random unsigned int in the range [min, max]
	u32 RandUInt(u32 min, u32 max);

	// Returns a random float in the range [min, max]
	f32 RandFloat(f32 min = 0.0f, f32 max = 1.0f);

	// Returns a random glm::vec2 where each component is in the range [min, max]
	glm::vec2 RandFloat2(f32 min = 0.0f, f32 max = 1.0f);

	// Returns a random glm::vec3 where each component is in the range [min, max]
	glm::vec3 RandFloat3(f32 min = 0.0f, f32 max = 1.0f);

	// Returns a random 2D direction vector with length provided (Note: uses glm::circularRand())
	glm::vec2 RandDir2D(f32 length = 1.0f);

	// Returns a random 2D direction vector with length provided (Note: uses glm::sphericalRand())
	glm::vec3 RandDir3D(f32 lenght = 1.0f);
}