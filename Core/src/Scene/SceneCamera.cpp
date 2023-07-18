#include "pch.h"
#include "SceneCamera.h"
#include "Math/Math.h"

void RecalculateProjection(SceneCamera& out)
{
	if (out.Spec.ProjectionType == ProjectionType::Orthographic)
	{
		float viewWidth = -out.Spec.OrthographicSize * out.Spec.AspectRatio * 0.5f;
		float viewHeight = -out.Spec.OrthographicSize * 0.5f;

		out.Projection = DirectX::XMMatrixOrthographicLH(viewWidth,
			viewHeight, out.Spec.OrthographicNear, out.Spec.OrthographicFar);
	}
}

void SceneCamera_Create(SceneCamera& out, const CameraSpecification& spec)
{
	out.Spec = spec;

	RecalculateProjection(out);
}

void SceneCamera_SetViewportSize(SceneCamera& out, uint32_t width, uint32_t height)
{
	BV_ASSERT(width > 0 && height > 0, "CameraViewPortSize - Invalid size");
	out.Spec.AspectRatio = (float)width / (float)height;
	RecalculateProjection(out);
}

const Mat& SceneCamera_GetProjectionMatrix(const SceneCamera& out)
{
	return out.Projection;
}