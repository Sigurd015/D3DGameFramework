#pragma once
#include "Math/Math.h"

struct BoxCollider2D
{
	Vec2 Offset = { 0.0f, 0.0f };
	Vec2 Size = { 1.0f, 1.0f };
};

struct CircleCollider2D
{
	Vec2 Offset = { 0.0f, 0.0f };
	float Radius = 1.0f;
};

bool Collisions_CC(const Vec2& center1, float radius1, const Vec2& center2, float radius2, Vec2* normal, float* depth);