#include "pch.h"
#include "Collisions.h"

bool Collisions_CC(const Vec2& center1, float radius1, const Vec2& center2, float radius2, Vec2* normal, float* depth)
{
	*normal = Vec2(0, 0);
	*depth = 0;

	float distence = Vec2Distance(center1, center2);
	float sumRadius = radius1 + radius2;

	if (distence > sumRadius)
		return false;

	*normal = Vec2Normalize(Vec2Sub(center2, center1));
	*depth = sumRadius - distence;

	return true;
}