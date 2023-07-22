#pragma once
#include "Math/Math.h"

enum ProjectionType
{
	Perspective = 0,
	Orthographic = 1
};

struct CameraSpecification
{
	ProjectionType ProjectionType = ProjectionType::Orthographic;

	float PerspectiveFOV = DirectX::XMConvertToRadians(45.0f);
	float PerspectiveNear = 0.01f, PerspectiveFar = 1000.0f;
	float OrthographicSize = 10.0f;
	float OrthographicNear = -100.0f, OrthographicFar = 100.0f;
	float AspectRatio = 0.0f;
};

struct SceneCamera
{
	CameraSpecification Spec;

	Mat Projection = DirectX::XMMatrixIdentity();
};

void SceneCamera_Create(SceneCamera& out, const CameraSpecification& spec);
const Mat& SceneCamera_GetProjectionMatrix(const SceneCamera& out);
void SceneCamera_SetViewportSize(SceneCamera& out, uint32_t width, uint32_t height);
void SceneCamera_SetProjectionType(SceneCamera& out, ProjectionType type);
void SceneCamera_SetOrthographicSize(SceneCamera& out, float size);