#pragma once
#include "glm/fwd.hpp"
#include <LEO/LeoEngine.h>

struct Transform
{
	glm::vec2 position      = glm::vec2(0.0f, 0.0f);
	f32       rotation      = 0.0f;  // in radians
};

struct Velocity
{
	glm::vec2 velocity      = glm::vec2(0.0f, 0.0f);
	f32       rotationSpeed = 0.0f; // in radians
};

struct Input
{
    enum Type : u8 {
        Forward       = 1 << 0,
        RightRotation = 1 << 1,
        LeftRotation  = 1 << 2,
        Shoot         = 1 << 3
    };

    u8 mask = 0;

    inline bool IsDown(Type t) const { return (mask & t) != 0; }
};

struct Ship
{
	f32 speed           = 0.0f; 
	f32 turnSpeed       = 0.0f; 
	u32 score           = 0;
};

struct LifeTime
{
	f32 timeLeft = 0;
};

struct Sphere
{
	f32 radius = 0.0f;
};

struct HitPoints
{
	u32 hp = 0;
};

#define MAX_VERTICES 16
struct Polygon
{
	u32       vertexCount                = 0;
	glm::vec2 baseShape[MAX_VERTICES]    = {};
	f32       approximateRadius          = 0.0f;
};


void UpdateTransform(Transform& t, const Velocity& v, f32 dt);
void BounceOffEdges(Transform& t, Velocity& v, f32 radius, f32 winW, f32 winH);
void ApplyMovementInput(const Input& input, Transform& t, f32 speed, f32 turnSpeed, f32 dt);

bool CheckCollisionSphere(const glm::vec2& a_pos, f32 a_radius, const glm::vec2& b_pos, f32 b_radius);
void ResolveCollisionSphere(glm::vec2& a_pos, glm::vec2& a_vel, f32 a_radius, 
	                          glm::vec2& b_pos, glm::vec2& b_vel, f32 b_radius);


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
Polygon GenerateRandomPolygon(u32 vertex_count, f32 vertex_dist_min, f32 vertex_dist_max);

/// <summary>
/// Renders a polygon by triangulating it into a fan of triangles.
/// Each vertex of the polygon is transformed by the given position and rotation,
/// then passed to the rendering system. The polygon is drawn in the specified color.
/// </summary>
/// <param name="t">Transform containing the position and rotation of the polygon.</param>
/// <param name="poly">Polygon data containing vertices in local space.</param>
/// <param name="color">Color used to render the polygon.</param>
void RenderPolygon(const Transform& t, const Polygon& poly, LEO::Color color);
