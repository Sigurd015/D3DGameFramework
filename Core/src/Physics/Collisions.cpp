#include "pch.h"
#include "Collisions.h"
#include "Renderer/Renderer2D.h"

void PointSegmentDistance(const Vec2& p, const Vec2& a, const Vec2& b, float* distanceSquared, Vec2* cp)
{
	Vec2 ab = Vec2Sub(b, a);
	Vec2 ap = Vec2Sub(p, a);

	float proj = Vec2Dot(ap, ab);
	float abLenSq = Vec2LengthSq(ab);
	float d = proj / abLenSq;

	if (d <= 0.0f)
	{
		*cp = a;
	}
	else if (d >= 1.0f)
	{
		*cp = b;
	}
	else
	{
		*cp = Vec2Add(a, Vec2MulFloat(ab, d));
	}

	*distanceSquared = Vec2DistanceSq(p, *cp);
}


bool Collisions_IntersectCircles(const Vec2& center1, float radius1, const Vec2& center2, float radius2, Vec2* normal, float* depth,
	Vec2* contactPoint, uint32_t* contactPointCount)
{
	float distence = Vec2Distance(center1, center2);
	float sumRadius = radius1 + radius2;

	if (distence > sumRadius)
		return false;

	Vec2 dir = Vec2Normalize(Vec2Sub(center2, center1));
	*normal = dir;
	*depth = sumRadius - distence;

	*contactPointCount = 1;
	contactPoint[0] = Vec2Add(center1, Vec2MulFloat(dir, radius1));

	//Debug
	Vec3 pos = { contactPoint[0].x,contactPoint[0].y,-0.1f };
	Vec3 rot = { 0,0,0 };
	Vec3 scale = { 0.5f,0.5f,1.0f };

	Mat trans = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
		* DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rot)))
		* DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	Renderer2D_DrawQuad(trans, { 1.0f,0,0,1.0f });

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

void FindPolygonsContactPoints(Vec2* vertices1, uint32_t count1, Vec2* vertices2, uint32_t count2,
	Vec2* contactPoint, uint32_t* contactPointCount)
{
	float minDistSq = FLT_MAX;

	for (int i = 0; i < count1; i++)
	{
		Vec2 p = vertices1[i];

		for (int j = 0; j < count2; j++)
		{
			Vec2 va = vertices2[j];
			Vec2 vb = vertices2[(j + 1) % count2];

			Vec2 contact = Vec2Zero;
			float distSq = 0;
			PointSegmentDistance(p, va, vb, &distSq, &contact);

			if (FloatNearlyEqual(distSq, minDistSq))
			{
				if (!Vec2NearlyEqual(contact, contactPoint[0]))
				{
					contactPoint[1] = contact;
					*contactPointCount = 2;
				}
			}
			else if (distSq < minDistSq)
			{
				minDistSq = distSq;
				*contactPointCount = 1;
				contactPoint[0] = contact;
			}
		}
	}

	for (int i = 0; i < count2; i++)
	{
		Vec2 p = vertices2[i];

		for (int j = 0; j < count1; j++)
		{
			Vec2 va = vertices1[j];
			Vec2 vb = vertices1[(j + 1) % count1];

			Vec2 contact = Vec2Zero;
			float distSq = 0;
			PointSegmentDistance(p, va, vb, &distSq, &contact);

			if (FloatNearlyEqual(distSq, minDistSq))
			{
				if (!Vec2NearlyEqual(contact, contactPoint[0]))
				{
					contactPoint[1] = contact;
					*contactPointCount = 2;
				}
			}
			else if (distSq < minDistSq)
			{
				minDistSq = distSq;
				*contactPointCount = 1;
				contactPoint[0] = contact;
			}
		}
	}
}

bool Collisions_IntersectPolygons(Vec2* vertices1, uint32_t count1, const Vec2& center1, Vec2* vertices2, uint32_t count2, const Vec2& center2,
	Vec2* normal, float* depth, Vec2* contactPoint, uint32_t* contactPointCount)
{
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

	Vec2 direction = Vec2Sub(center2, center1);

	if (Vec2Dot(direction, *normal) < 0.0f)
	{
		*normal = Vec2MulFloat(*normal, -1);
	}

	FindPolygonsContactPoints(vertices1, count1, vertices2, count2, contactPoint, contactPointCount);

	//Debug
	{
		for (size_t i = 0; i < *contactPointCount; i++)
		{
			Vec3 pos = { contactPoint[i].x,contactPoint[i].y,-0.1f };
			Vec3 rot = { 0,0,0 };
			Vec3 scale = { 0.5f,0.5f,1.0f };

			Mat trans = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
				* DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rot)))
				* DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

			Renderer2D_DrawQuad(trans, { 1.0f,0,0,1.0f });
		}
	}

	return true;
}

void ProjectCircle(Vec2& center, float radius, Vec2& axis, float* min, float* max)
{
	Vec2 direction = Vec2Normalize(axis);
	Vec2 directionAndRadius = Vec2MulFloat(direction, radius);

	Vec2 p1 = Vec2Add(center, directionAndRadius);
	Vec2 p2 = Vec2Sub(center, directionAndRadius);

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

bool Collisions_IntersectCirclePolygon(Vec2& circleCenter, float circleRadius, Vec2* vertices, uint32_t count, const Vec2& polygonCenter,
	Vec2* normal, float* depth, Vec2* contactPoint, uint32_t* contactPointCount)
{
	*contactPointCount = 1;
	float minDistSq = FLT_MAX;

	Vec2 axis = Vec2Zero;
	float axisDepth = 0;
	float minA, maxA, minB, maxB;

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

		Vec2 contact = Vec2Zero;
		float distSq = 0;
		PointSegmentDistance(circleCenter, va, vb, &distSq, &contact);

		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			contactPoint[0] = contact;
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

	Vec2 direction = Vec2Sub(polygonCenter, circleCenter);

	if (Vec2Dot(direction, *normal) < 0)
	{
		*normal = Vec2MulFloat(*normal, -1);
	}

	//Debug
	{
		for (size_t i = 0; i < *contactPointCount; i++)
		{
			Vec3 pos = { contactPoint[i].x,contactPoint[i].y,-0.1f };
			Vec3 rot = { 0,0,0 };
			Vec3 scale = { 0.5f,0.5f,1.0f };

			Mat trans = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
				* DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rot)))
				* DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

			Renderer2D_DrawQuad(trans, { 1.0f,0,0,1.0f });
		}
	}
	
	return true;
}

bool Collisions_IntersectAABB(const AABB& aabb1, const AABB& aabb2)
{
	if (aabb1.Max.x <= aabb2.Min.x || aabb2.Max.x <= aabb1.Min.x ||
		aabb1.Max.y <= aabb2.Min.y || aabb2.Max.y <= aabb1.Min.y)
	{
		return false;
	}

	return true;
}