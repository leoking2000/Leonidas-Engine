#pragma once
#include <LEO/LeoEngine.h>

struct Transform
{
	glm::vec2 position      = glm::vec2(0.0f, 0.0f);
	f32       rotation      = 0.0f;  // in radians
	
	glm::vec2 velocity      = glm::vec2(0.0f, 0.0f);
	f32       rotationSpeed = 0.0f; // in radians
};

#define MAX_VERTICES 16
struct Polygon
{
	u32       vertexCount;
	glm::vec2 baseShape[MAX_VERTICES];
	f32       approximentRadius;
};

struct Input
{
	/// <summary>
	/// x -> up key down
	/// y -> down key down
	/// z -> right key down
	/// a -> left key down
	/// </summary>
	glm::bvec4 inputs;
};

/// <summary>
/// Generates a random polygon with the specified number of vertices.
/// Each vertex is placed at an angle step around the origin with a random
/// distance between `vertex_dist_min` and `vertex_dist_max`.  
/// Validates input constraints (minimum 3 vertices, within MAX_VERTICES,
/// and proper distance ranges).  
/// Returns a `Polygon` with populated vertex positions and an average radius.
/// </summary>
/// <param name="vertex_count">Number of vertices to generate (must be between 3 and MAX_VERTICES).</param>
/// <param name="vertex_dist_min">Minimum distance from the origin for any vertex (must be > 0).</param>
/// <param name="vertex_dist_max">Maximum distance from the origin for any vertex (must be > vertex_dist_min).</param>
/// <returns>A `Polygon` object containing the generated vertices and its average radius.</returns>
Polygon GenerateRendomPolygon(u32 vertex_count, f32 vertex_dist_min, f32 vertex_dist_max);


/// <summary>
/// Checks whether two polygons are colliding by approximating them as circles.
/// The circle radius is taken from the polygon's estimated radius, and collision
/// is determined by comparing the squared distance between centers with the squared
/// sum of their radii.
/// </summary>
/// <param name="t_a">Transform of the first polygon (position & velocity).</param>
/// <param name="poly_a">Polygon data of the first polygon (approximated radius used).</param>
/// <param name="t_b">Transform of the second polygon (position & velocity).</param>
/// <param name="poly_b">Polygon data of the second polygon (approximated radius used).</param>
/// <returns>True if the polygons (treated as circles) are colliding, false otherwise.</returns>
bool CheckColitionPolygons(const Transform& t_a, const Polygon& poly_a,
							  const Transform& t_b, const Polygon& poly_b);

// <summary>
/// Resolves a collision between two polygons by approximating them as circles.
/// Mass is assumed proportional to the circle radius. The function separates the
/// circles to prevent overlap and applies an elastic collision response along the
/// collision normal, updating their velocities based on relative mass.
/// </summary>
/// <param name="t_a">Transform of the first polygon (position & velocity).</param>
/// <param name="poly_a">Polygon data of the first polygon (approximated radius used).</param>
/// <param name="t_b">Transform of the second polygon (position & velocity).</param>
/// <param name="poly_b">Polygon data of the second polygon (approximated radius used).</param>
void ResolveCollisionPolygons(Transform& t_a, const Polygon& poly_a,
								Transform& t_b, const Polygon& poly_b);

/// <summary>
/// Renders a polygon by triangulating it into a fan of triangles.
/// Each vertex of the polygon is transformed by the given position and rotation,
/// then passed to the rendering system. The polygon is drawn in the specified color.
/// </summary>
/// <param name="t">Transform containing the position and rotation of the polygon.</param>
/// <param name="poly">Polygon data containing vertices in local space.</param>
/// <param name="color">Color used to render the polygon.</param>
void RenderPolygon(const Transform& t, const Polygon& poly, LEO::Color color);
