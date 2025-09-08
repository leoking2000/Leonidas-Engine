#pragma once
#include <LEO/LeoEngine.h>

struct Transform
{
	glm::vec2 position      = glm::vec2(0.0f, 0.0f);
	f32       rotation      = 0.0f;  // in radians
	
	glm::vec2 velocity      = glm::vec2(0.0f, 0.0f);
	f32       rotationSpeed = 0.0f; // in radians
};

struct Input
{
	/// <summary>
	/// x -> forward key down
	/// y -> backward key down
	/// z -> right rotation key down
	/// a -> left rotation key down
	/// </summary>
	glm::bvec4 inputs;
};

#define MAX_VERTICES 16
struct Polygon
{
	u32       vertexCount;
	glm::vec2 baseShape[MAX_VERTICES];
	f32       approximateRadius;
};


void UpdateTransform(Transform& t, f32 dt);

void BounceOffEdges(Transform& t, const Polygon& poly, f32 winW, f32 winH);

/// <summary>
/// Applies input to the transform by setting its velocity and rotation speed.
/// Up/Down keys move the player forward/backward based on its facing direction.
/// Left/Right keys rotate the player by setting its rotation speed.
/// </summary>
/// <param name="input">Input state (x=up, y=down, z=right, a=left).</param>
/// <param name="t">Transform to update (velocity and rotationSpeed are set).</param>
/// <param name="speed">Movement speed of the player.</param>
/// <param name="rotationSpeed">Angular speed applied when rotating left/right.</param>
void ApplyInput(const Input& input, Transform& t, f32 speed, f32 rotationSpeed, f32 maxSpeed);


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
