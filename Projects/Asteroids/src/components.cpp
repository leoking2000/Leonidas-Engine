#include <limits>
#include <array>
#include "components.h"

inline void WrapAngle(f32& angle)
{
	const float TWO_PI = glm::two_pi<f32>();
	angle = fmod(angle, TWO_PI);
	if (angle < 0.0f)
		angle += TWO_PI;
}

void UpdateTransform(Transform& t, f32 dt)
{
	t.position += t.velocity * dt;
	t.rotation += t.rotationSpeed * dt;
	WrapAngle(t.rotation);
}

void BounceOffEdges(Transform& t, const Polygon& poly, f32 winW, f32 winH)
{
	f32 r = poly.approximateRadius;
	if (t.position.x - r < 0) { t.position.x = r; t.velocity.x *= -1; }
	if (t.position.x + r > winW) { t.position.x = winW - r; t.velocity.x *= -1; }
	if (t.position.y - r < 0) { t.position.y = r; t.velocity.y *= -1; }
	if (t.position.y + r > winH) { t.position.y = winH - r; t.velocity.y *= -1; }
}

void ApplyInput(const Input& input, Transform& t, f32 speed, f32 rotationSpeed, f32 maxSpeed)
{
	glm::vec2 dir(0.0f);

	// Movement (forward/backward relative to current rotation)
	if (input.inputs.x) dir.y += 1.0f; // forward
	if (input.inputs.y) dir.y -= 1.0f; // backward

	if (dir != glm::vec2(0.0f))
	{
		// Forward vector: 0 rotation = up, Y+ = down
		glm::vec2 forward(-sin(t.rotation), -cos(t.rotation));
		t.velocity += forward * dir.y * speed;

		if (glm::length2(t.velocity) > maxSpeed * maxSpeed)
		{
			t.velocity = glm::normalize(t.velocity) * maxSpeed;
		}	
	}

	// Rotation
	t.rotationSpeed = 0.0f;
	if (input.inputs.z) t.rotationSpeed -= rotationSpeed;  // rotate right
	if (input.inputs.a) t.rotationSpeed += rotationSpeed;  // rotate left
}


Polygon GenerateRendomPolygon(u32 vertex_count, f32 vertex_dist_min, f32 vertex_dist_max)
{
	LEOASSERTF(vertex_count >= 3u, "Max vertex count can't be less that three, {} provided", vertex_count);
	LEOASSERTF(vertex_count <= MAX_VERTICES, "Max vertex count can't be more than {}, {} provided", MAX_VERTICES, vertex_count);

	LEOASSERT(vertex_dist_min > 0.0f, "Minimum vertex distance should be greter that 0").LEOWATCH(vertex_dist_min);
	LEOASSERT(vertex_dist_max > 0.0f, "Maximum vertex distance should be greter that 0").LEOWATCH(vertex_dist_max);

	LEOASSERT(vertex_dist_max > vertex_dist_min,
		"Maximum vertex distance should be greter that the Minimum vertex distance")
		.LEOWATCH(vertex_dist_min)
		.LEOWATCH(vertex_dist_max);

	Polygon poly = {};

	poly.vertexCount = vertex_count;

	f32 step = 360.0f / vertex_count;

	f32 radius_sum = 0.0f;
	
	for (u32 i = 0; i < poly.vertexCount; i++) 
	{
		f32 angle = glm::radians(step * i);
		f32 dist  = LEO::RandFloat(vertex_dist_min, vertex_dist_max);

		poly.baseShape[i] = { glm::cos(angle) * dist, glm::sin(angle) * dist };

		radius_sum += dist;
	}

	poly.approximateRadius = radius_sum / vertex_count;

	return poly;
}

bool CheckColitionPolygons(const Transform& t_a, const Polygon& poly_a,
	                       const Transform& t_b, const Polygon& poly_b)
{
	// Treat polygons as circles with given radius
	float dist2 = glm::length2(t_a.position - t_b.position);
	float radiusSum = poly_a.approximateRadius + poly_b.approximateRadius;
	return dist2 <= (radiusSum * radiusSum);
}

void ResolveCollisionPolygons(Transform& t_a, const Polygon& poly_a,
	                          Transform& t_b, const Polygon& poly_b)
{
	if (!CheckColitionPolygons(t_a, poly_a, t_b, poly_b))
		return;

	glm::vec2 diff = t_b.position - t_a.position;
	float dist = glm::length(diff);
	if (dist == 0.0f) return;


	glm::vec2 normal = diff / dist;
	float penetration = (poly_a.approximateRadius + poly_b.approximateRadius) - dist;


	// Mass proportional to radius
	float massA = poly_a.approximateRadius;
	float massB = poly_b.approximateRadius;
	float totalMass = massA + massB;


	// Separate based on relative mass
	t_a.position -= normal * (penetration * (massB / totalMass));
	t_b.position += normal * (penetration * (massA / totalMass));


	// Velocity resolution (elastic collision along normal)
	float va = glm::dot(t_a.velocity, normal);
	float vb = glm::dot(t_b.velocity, normal);


	float newVa = (va * (massA - massB) + 2.0f * massB * vb) / totalMass;
	float newVb = (vb * (massB - massA) + 2.0f * massA * va) / totalMass;


	t_a.velocity += (newVa - va) * normal;
	t_b.velocity += (newVb - vb) * normal;
}

void RenderPolygon(const Transform& t, const Polygon& poly, LEO::Color color)
{
	LEOASSERTF(poly.vertexCount >= 3u, "Polygon vertex count can't be less that three, {} provided", poly.vertexCount);

	// Precompute rotation
	float c = glm::cos(t.rotation);
	float s = glm::sin(t.rotation);
	glm::mat2 rot = { {c, -s}, {s, c} };

	// Transform vertices once
	std::array<glm::vec2, MAX_VERTICES> transformed;
	for (u32 i = 0; i < poly.vertexCount; i++) {
		transformed[i] = t.position + rot * poly.baseShape[i];
	}

	// Draw as triangle fan
	const glm::vec2& v0 = transformed[0];
	for (u32 i = 1; i < poly.vertexCount - 1; i++) {
		LEO::RenderTriangle(transformed[i + 1], transformed[i], v0, color);
	}
}
