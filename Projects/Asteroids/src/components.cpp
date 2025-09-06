#include <limits>
#include <array>
#include "components.h"


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
	
	for (i32 i = 0; i < poly.vertexCount; i++) 
	{
		f32 angle = glm::radians(step * i);
		f32 dist  = LEO::RandFloat(vertex_dist_min, vertex_dist_max);

		poly.baseShape[i] = { glm::cos(angle) * dist, glm::sin(angle) * dist };

		radius_sum += dist;
	}

	poly.approximentRadius = radius_sum / vertex_count;

	return poly;
}

bool CheckColitionPolygons(const Transform& t_a, const Polygon& poly_a,
	                       const Transform& t_b, const Polygon& poly_b)
{
	// Treat polygons as circles with given radius
	float dist2 = glm::length2(t_a.position - t_b.position);
	float radiusSum = poly_a.approximentRadius + poly_b.approximentRadius;
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
	float penetration = (poly_a.approximentRadius + poly_b.approximentRadius) - dist;


	// Mass proportional to radius
	float massA = poly_a.approximentRadius;
	float massB = poly_b.approximentRadius;
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
