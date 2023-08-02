#include "pch.h"
#include "SceneCamera.h"
#include "Math/Math.h"

void RecalculateProjection(SceneCamera& out)
{
	if (out.Spec.ProjectionType == ProjectionType::Orthographic)
	{
		float orthoLeft = -out.Spec.OrthographicSize * out.Spec.AspectRatio * 0.5f;
		float orthoRight = -orthoLeft;
		float orthoBottom = -out.Spec.OrthographicSize * 0.5f;
		float orthoTop = -orthoBottom;

		out.Projection = DirectX::XMMatrixOrthographicOffCenterLH(orthoLeft,
			orthoRight, orthoBottom, orthoTop, out.Spec.OrthographicNear, out.Spec.OrthographicFar);
	}
	else
	{
		out.Projection = DirectX::XMMatrixPerspectiveFovLH(out.Spec.PerspectiveFOV,
			out.Spec.AspectRatio, out.Spec.PerspectiveNear, out.Spec.PerspectiveFar);
	}
}

void SceneCamera_Create(SceneCamera& out, const CameraSpecification& spec)
{
	out.Spec = spec;

	RecalculateProjection(out);
}

void SceneCamera_SetViewportSize(SceneCamera& out, uint32_t width, uint32_t height)
{
	CORE_ASSERT(width > 0 && height > 0, "CameraViewPortSize - Invalid size");
	out.Spec.AspectRatio = (float)width / (float)height;
	RecalculateProjection(out);
}

void SceneCamera_SetProjectionType(SceneCamera& out, ProjectionType type)
{
	out.Spec.ProjectionType = type;
	RecalculateProjection(out);
}

void SceneCamera_SetOrthographicSize(SceneCamera& out, float size)
{
	out.Spec.OrthographicSize = size;
	RecalculateProjection(out);
}

const Mat& SceneCamera_GetProjectionMatrix(const SceneCamera& out)
{
	return out.Projection;
}