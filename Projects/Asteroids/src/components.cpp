#include <array>
#include "components.h"

inline void WrapAngle(f32& angle)
{
	const float TWO_PI = glm::two_pi<f32>();
	angle = fmod(angle, TWO_PI);
	if (angle < 0.0f)
		angle += TWO_PI;
}

void UpdateTransform(Transform& t, const Velocity& v, f32 dt)
{
	t.position += v.velocity * dt;
	t.rotation += v.rotationSpeed * dt;
	WrapAngle(t.rotation);
}

void BounceOffEdges(Transform& t, Velocity& v, f32 radius, f32 winW, f32 winH)
{
	if (t.position.x - radius < 0) { t.position.x = radius; v.velocity.x *= -1; }
	if (t.position.x + radius > winW) { t.position.x = winW - radius; v.velocity.x *= -1; }
	if (t.position.y - radius < 0) { t.position.y = radius; v.velocity.y *= -1; }
	if (t.position.y + radius > winH) { t.position.y = winH - radius; v.velocity.y *= -1; }
}

void ApplyMovementInput(const Input& input, Transform& t, f32 speed, f32 turnSpeed, f32 dt)
{
	glm::vec2 direction = glm::vec2{ glm::cos(t.rotation), -glm::sin(t.rotation) };

    if (input.IsDown(Input::Forward)) {
        t.position += direction * speed * dt;
    }

    if (input.IsDown(Input::RightRotation)) {
        t.rotation -= turnSpeed * dt;
    }

    if (input.IsDown(Input::LeftRotation)) {
        t.rotation += turnSpeed * dt;
    }

    WrapAngle(t.rotation); // keep rotation bounded
}


Polygon GenerateRandomPolygon(u32 vertex_count, f32 vertex_dist_min, f32 vertex_dist_max)
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

bool CheckCollisionSphere(const glm::vec2& a_pos, f32 a_radius, const glm::vec2& b_pos, f32 b_radius)
{
	// Treat polygons as circles with given radius
	f32 dist2 = glm::length2(a_pos - b_pos);
	f32 radiusSum = a_radius + b_radius;
	return dist2 <= (radiusSum * radiusSum);
}

void ResolveCollisionSphere(glm::vec2& a_pos, glm::vec2& a_vel, f32 a_radius, 
	                          glm::vec2& b_pos, glm::vec2& b_vel, f32 b_radius)
{
	if (!CheckCollisionSphere(a_pos, a_radius, b_pos, b_radius))
		return;

	glm::vec2 diff = b_pos - a_pos;
	float dist = glm::length(diff);
	if (dist == 0.0f) return;


	glm::vec2 normal = diff / dist;
	float penetration = (a_radius + b_radius) - dist;


	// Mass proportional to radius
	float massA = a_radius;
	float massB = b_radius;
	float totalMass = massA + massB;


	// Separate based on relative mass
	a_pos -= normal * (penetration * (massB / totalMass));
	b_pos += normal * (penetration * (massA / totalMass));


	// Velocity resolution (elastic collision along normal)
	float va = glm::dot(a_vel, normal);
	float vb = glm::dot(b_vel, normal);


	float newVa = (va * (massA - massB) + 2.0f * massB * vb) / totalMass;
	float newVb = (vb * (massB - massA) + 2.0f * massA * va) / totalMass;


	a_vel += (newVa - va) * normal;
	b_vel += (newVb - vb) * normal;
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
