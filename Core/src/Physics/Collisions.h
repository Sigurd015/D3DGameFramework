#pragma once
#include "Math/Math.h"

struct BoxCollider2D
{
	Vec2 Offset = { 0.0f, 0.0f };
	Vec2 Size = { 0.0f, 0.0f };

	Vec2 Vertices[4] = { { 0.0f, 0.0f }, { 0.0f, 0.0f },{ 0.0f, 0.0f }, { 0.0f, 0.0f } };
};

struct CircleCollider2D
{
	Vec2 Offset = { 0.0f, 0.0f };
	float Radius = 0.0f;
};

struct AABB
{
	Vec2 Min = { 0.0f, 0.0f };
	Vec2 Max = { 0.0f, 0.0f };
};

struct ContactParams
{
	float Depth = FLT_MAX;
	Vec2 Normal = Vec2Zero;
	Vec2 ContactPoint[2] = { Vec2Zero,Vec2Zero };
	uint32_t ContactPointCount = 0;
};

bool Collisions_IntersectCircles(const Vec2& center1, float radius1, const Vec2& center2, float radius2, ContactParams& contactParams);
bool Collisions_IntersectPolygons(Vec2* vertices1, uint32_t count1, const Vec2& center1, Vec2* vertices2, uint32_t count2, const Vec2& center2,
	ContactParams& contactParams);
bool Collisions_IntersectCirclePolygon(Vec2& circleCenter, float circleRadius, Vec2* vertices, uint32_t count, const Vec2& polygonCenter,
	ContactParams& contactParams);
bool Collisions_IntersectAABB(const AABB& aabb1, const AABB& aabb2);
bool Collisions_RayCastCircles(const Vec2& rayOrigin, const Vec2& rayDirection, const Vec2& center, float radius, float& distance);
bool Collisions_RayCastPolygons(const Vec2& rayOrigin, const Vec2& rayDirection, Vec2* vertices, uint32_t count, const Vec2& center, float& distance);