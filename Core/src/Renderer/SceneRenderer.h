#pragma once
#include "ConstantBuffer.h"
#include "RenderPass.h"
#include "Scene/Components.h"
#include "Material.h"

// TODO: Temporary
struct Mesh;
struct Environment;
struct MeshComponent;

void SceneRenderer_Init();
void SceneRenderer_Shutdown();
void SceneRenderer_SetViewportSize(uint32_t width, uint32_t height);

void SceneRenderer_BeginScene(const Environment& environment);
void SceneRenderer_EndScene();

void ScnenRenderer_SubmitStaticMesh(const Mat4& transform, MeshComponent& meshComponent, Material* overrideMaterial = nullptr);

const RenderPass& SceneRenderer_GetFinalPass();
//----- GBuffer Debugging -----
Image2D* SceneRenderer_GetGBufferAlbedo();
Image2D* SceneRenderer_GetGBufferMRE();
Image2D* SceneRenderer_GetGBufferNormal();
Image2D* SceneRenderer_GetGBufferPosition();
Image2D* SceneRenderer_GetDirShadowMap();

struct SceneRendererStatistics
{
	uint32_t DrawCalls = 0;
};
void SceneRenderer_ResetStats();
SceneRendererStatistics SceneRenderer_GetStats();