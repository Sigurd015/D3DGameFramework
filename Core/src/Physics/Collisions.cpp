#include "pch.h"
#include "Collisions.h"
#include "Renderer/Renderer2D.h"
bool IntersectCircles(const Vec2& center1, float radius1, const Vec2& center2, float radius2, Vec2* normal, float* depth)
{
	*normal = Vec2Zero;
	*depth = 0;

	float distence = Vec2Distance(center1, center2);
	float sumRadius = radius1 + radius2;

	if (distence > sumRadius)
		return false;

	*normal = Vec2Normalize(Vec2Sub(center2, center1));
	*depth = sumRadius - distence;

	return true;
}

void ProjectVertices(Vec2* vertices, uint32_t count, Vec2& axis, float* min, float* max)
{
	*min = FLT_MAX;
	*max = FLT_MIN;

	for (int i = 0; i < count; i++)
	{
		Vec2 v = vertices[i];
		float proj = Vec2Dot(v, axis);

		if (proj < *min)
			*min = proj;

		if (proj > *max)
			*max = proj;
	}
}

Vec2 FindArithmeticMean(Vec2* vertices, uint32_t count)
{
	float sumX = 0;
	float sumY = 0;

	for (int i = 0; i < count; i++)
	{
		Vec2 v = vertices[i];
		sumX += v.x;
		sumY += v.y;
	}

	Vec2 result = { sumX / (float)count, sumY / (float)count };
	return result;
}

bool IntersectPolygons(Vec2* vertices1, uint32_t count1, Vec2* vertices2, uint32_t count2, Vec2* normal, float* depth)
{
	*normal = Vec2Zero;
	*depth = FLT_MAX;

	{
		for (int i = 0; i < count1; i++)
		{
			Vec2 va = vertices1[i];
			Vec2 vb = vertices1[(i + 1) % count1];

			Vec2 edge = Vec2Sub(vb, va);
			Vec2 axis = { -edge.y, edge.x };
			axis = Vec2Normalize(axis);

			float minA, maxA, minB, maxB;
			ProjectVertices(vertices1, count1, axis, &minA, &maxA);
			ProjectVertices(vertices2, count2, axis, &minB, &maxB);

			if (minA >= maxB || minB >= maxA)
			{
				return false;
			}

			float axisDepth = FloatMin(maxB - minA, maxA - minB);

			if (axisDepth < *depth)
			{
				*depth = axisDepth;
				*normal = axis;
			}
		}
	}

	{
		for (int i = 0; i < count2; i++)
		{
			Vec2 va = vertices2[i];
			Vec2 vb = vertices2[(i + 1) % count2];

			Vec2 edge = Vec2Sub(vb, va);
			Vec2 axis = { -edge.y, edge.x };
			axis = Vec2Normalize(axis);

			float minA, maxA, minB, maxB;
			ProjectVertices(vertices1, count1, axis, &minA, &maxA);
			ProjectVertices(vertices2, count2, axis, &minB, &maxB);

			if (minA >= maxB || minB >= maxA)
			{
				return false;
			}

			float axisDepth = FloatMin(maxB - minA, maxA - minB);

			if (axisDepth < *depth)
			{
				*depth = axisDepth;
				*normal = axis;
			}
		}
	}

	Vec2 centerA = FindArithmeticMean(vertices1, count1);
	Vec2 centerB = FindArithmeticMean(vertices2, count2);

	Vec2 direction = Vec2Sub(centerB, centerA);

	if (Vec2Dot(direction, *normal) < 0)
	{
		*normal = Vec2MulFloat(*normal, -1);
	}

	return true;
}

void ProjectCircle(Vec2& center, float radius, Vec2& axis, float* min, float* max)
{
	Vec2 direction = Vec2Normalize(axis);
	Vec2 directionAndRadius = Vec2MulFloat(direction, radius);

	Vec2 p1 = Vec2Add(center, directionAndRadius);
	Vec2 p2 = Vec2Add(center, directionAndRadius);

	*min = Vec2Dot(p1, axis);
	*max = Vec2Dot(p2, axis);

	if (*min > *max)
	{
		// swap the min and max values.
		float t = *min;
		*min = *max;
		*max = t;
	}
}

int FindClosestPointOnPolygon(Vec2& circleCenter, Vec2* vertices, uint32_t count)
{
	int result = -1;
	float minDistance = FLT_MAX;

	for (int i = 0; i < count; i++)
	{
		Vec2 v = vertices[i];
		float distance = Vec2Distance(v, circleCenter);

		if (distance < minDistance)
		{
			minDistance = distance;
			result = i;
		}
	}

	return result;
}

bool IntersectCirclePolygon(Vec2& circleCenter, float circleRadius, Vec2* vertices, uint32_t count, Vec2* normal, float* depth)
{
	*normal = Vec2Zero;
	*depth = FLT_MAX;

	Vec2 axis = Vec2Zero;
	float axisDepth = 0;
	float minA, maxA, minB, maxB;

	//Debug
	Vec3 translation = Vec3(circleCenter.x, circleCenter.y, 0.001f);
	float radius = circleRadius * 2.05f;
	Vec3 scale = Vec3MulVec3(Vec3(radius, radius, radius), Vec3(1.0f, 1.0f, 1.0f));

	Mat transform = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
		* DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);

	Renderer2D_DrawCircle(transform, Vec4(0, 1, 0, 1), 0.01f);

	for (int i = 0; i < count; i++)
	{
		Vec2 va = vertices[i];
		Vec2 vb = vertices[(i + 1) % count];

		Vec2 edge = Vec2Sub(vb, va);
		axis = { -edge.y, edge.x };
		axis = Vec2Normalize(axis);

		ProjectVertices(vertices, count, axis, &minA, &maxA);
		ProjectCircle(circleCenter, circleRadius, axis, &minB, &maxB);

		if (minA >= maxB || minB >= maxA)
		{
			return false;
		}

		axisDepth = FloatMin(maxB - minA, maxA - minB);

		if (axisDepth < *depth)
		{
			*depth = axisDepth;
			*normal = axis;
		}
	}

	int cpIndex = FindClosestPointOnPolygon(circleCenter, vertices, count);
	Vec2 cp = vertices[cpIndex];

	axis = Vec2Sub(cp, circleCenter);
	axis = Vec2Normalize(axis);

	ProjectVertices(vertices, count, axis, &minA, &maxA);
	ProjectCircle(circleCenter, circleRadius, axis, &minB, &maxB);

	if (minA >= maxB || minB >= maxA)
	{
		return false;
	}

	axisDepth = FloatMin(maxB - minA, maxA - minB);

	if (axisDepth < *depth)
	{
		*depth = axisDepth;
		*normal = axis;
	}

	Vec2 polygonCenter = FindArithmeticMean(vertices, count);

	Vec2 direction = Vec2Sub(polygonCenter, circleCenter);

	if (Vec2Dot(direction, *normal) < 0)
	{
		*normal = Vec2MulFloat(*normal, -1);
	}

	return true;
}