#include "pch.h"
#include "SceneRenderer.h"
#include "Asset/AssetManager.h"

#define MAX_POINT_LIGHT 32
#define MAX_SPOT_LIGHT 32

struct SceneRendererData
{
	struct DirectionalLight
	{
		Vec3 Radiance = { 1.0f,1.0f,1.0f };
		float Intensity = 0.0f;
		Vec3 Direction = { 0.0f, 0.0f, 0.0f };

		// Padding
		char padding[4];
	};

	struct PointLight
	{
		Vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Intensity = 0.0f;
		Vec3 Radiance = { 1.0f,1.0f,1.0f };
		float Radius;
		float Falloff;

		// Padding
		char padding[12];
	};

	struct SpotLight
	{
		Vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Intensity = 0.0f;
		Vec3 Radiance = { 1.0f,1.0f,1.0f };
		float AngleAttenuation;
		Vec3 Direction = { 0.0f, 0.0f, 0.0f };
		float Range;
		float Angle;
		float Falloff;

		// Padding
		char padding[8];
	};

	struct MaterialData
	{
		Vec3 Albedo;
		float Emission;
		float Metalness;
		float Roughness;
		bool UseNormalMap;

		// Padding
		// Notice : bool is 1 byte in C++ but 4 bytes in HLSL
		char padding[7];
	};

	struct CBModel
	{
		Mat4 Transform;
		MaterialData Material;
	};

	struct CBCamera
	{
		Mat4 ViewProj;
		Vec3 CameraPosition;

		// Padding
		char padding[4];
	};

	struct CBScene
	{
		float SkyLightIntensity = 0.0f;

		// Padding
		char padding[12];

		DirectionalLight Light;
	};

	struct CBPointLight
	{
		uint32_t Count = 0;

		// Padding
		char padding[12];

		PointLight PointLights[MAX_POINT_LIGHT]{};
	};

	struct CBSpotLight
	{
		uint32_t Count = 0;

		// Padding
		char padding[12];

		SpotLight SpotLights[MAX_SPOT_LIGHT]{};
	};

	struct CBDirShadow
	{
		Mat4 LightViewProj;
		uint32_t ShadowType = 0;

		// Padding
		char padding[12];
	};

	struct CBPointShadow
	{
		Mat4 LightViewProj[6];
		Vec3 LightPosition;
		float FarPlane = 0.0f;
		uint32_t ShadowType = 0;

		// Padding
		char padding[12];
	};

	Environment* SceneEnvironment;

	CBModel ModelData;
	CBCamera CameraData;
	CBScene SceneData;
	CBPointLight PointLightData;
	CBSpotLight SpotLightData;
	CBDirShadow DirShadowData;
	CBPointShadow PointShadowData;

	ConstantBuffer ModelDataBuffer;
	ConstantBuffer CameraDataBuffer;
	ConstantBuffer SceneDataBuffer;
	ConstantBuffer PointLightDataBuffer;
	ConstantBuffer SpotLightDataBuffer;
	ConstantBuffer DirShadowDataBuffer;
	ConstantBuffer PointShadowDataBuffer;

	RenderPass DirShadowMapPass;
	RenderPass PointShadowMapPass;
	RenderPass DeferredGeoPass;
	RenderPass DeferredLightingPass;
	RenderPass SkyboxPass;
	RenderPass CompositePass;

	Material* DefaultMaterial;

	struct GeoDrawCommand
	{
		Mesh* Mesh;
		uint32_t SubmeshIndex;
		Material* Material;

		CBModel ModelData;
	};
	List DrawCommands;
	SceneRendererStatistics RendererStats;
};
static SceneRendererData s_Data;


void SceneRenderer_Init()
{
	s_Data = {};

	ConstantBuffer_Create(s_Data.ModelDataBuffer, sizeof(SceneRendererData::CBModel));
	ConstantBuffer_Create(s_Data.CameraDataBuffer, sizeof(SceneRendererData::CBCamera));
	ConstantBuffer_Create(s_Data.SceneDataBuffer, sizeof(SceneRendererData::CBScene));
	ConstantBuffer_Create(s_Data.PointLightDataBuffer, sizeof(SceneRendererData::CBPointLight));
	ConstantBuffer_Create(s_Data.SpotLightDataBuffer, sizeof(SceneRendererData::CBSpotLight));
	ConstantBuffer_Create(s_Data.DirShadowDataBuffer, sizeof(SceneRendererData::CBDirShadow));
	ConstantBuffer_Create(s_Data.PointShadowDataBuffer, sizeof(SceneRendererData::CBPointShadow));

	VertexBufferLayoutEmelent GeoLayout[5] = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float3, "a_Normal" },
		{ ShaderDataType::Float3, "a_Tangent" },
		{ ShaderDataType::Float3, "a_Bitangent" },
		{ ShaderDataType::Float2, "a_TexCoord" },
	};

	VertexBufferLayoutEmelent FullScreenQuadLayout[2] = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" },
	};

	VertexBufferLayoutEmelent SkyboxLayout[1] = {
		{ ShaderDataType::Float3, "a_Position" },
	};

	// Deferred Geometry Pass
	{
		PipelineSpecification pipelineSpec;
		{
			FramebufferSpecification spec;

			// TODO: Implement Framebuffer
			Framebuffer_Create(pipelineSpec.TargetFramebuffer, spec);
		}
		{
			pipelineSpec.Layout = { GeoLayout, 5 };
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("DeferredGeometry", true);
			pipelineSpec.BackfaceCulling = true;
			pipelineSpec.DepthTest = true;
			pipelineSpec.Topology = PrimitiveTopology_Triangles;
			pipelineSpec.DepthOperator = DepthCompareOperator_Less;

			RenderPassSpecification renderPassSpec;
			Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
			RenderPass_Create(s_Data.DeferredGeoPass, renderPassSpec);

			// Create default material
			//Material_Create(s_Data.DefaultMaterial, pipelineSpec.Shader);
		}
	}

	// Dir Shadow Map Pass
	{
		PipelineSpecification pipelineSpec;
		{
			FramebufferSpecification spec;

			// TODO: Implement Framebuffer
			Framebuffer_Create(pipelineSpec.TargetFramebuffer, spec);
		}
		{
			// Notice: Use the same layout as DeferredGeoPass, 
			// because transparent objects need check alpha value from Albedo Texture
			pipelineSpec.Layout = { GeoLayout, 5 };
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("DirShadowMap", true);
			pipelineSpec.BackfaceCulling = true;
			pipelineSpec.DepthTest = true;
			pipelineSpec.Topology = PrimitiveTopology_Triangles;
			pipelineSpec.DepthOperator = DepthCompareOperator_Less;

			RenderPassSpecification renderPassSpec;
			Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
			RenderPass_Create(s_Data.DirShadowMapPass, renderPassSpec);
		}
	}
	// Point Shadow Map Pass
	{
		PipelineSpecification pipelineSpec;
		{
			FramebufferSpecification spec;

			// TODO: Implement Framebuffer
			Framebuffer_Create(pipelineSpec.TargetFramebuffer, spec);
		}
		{
			// Notice: Use the same layout as DeferredGeoPass, 
			// because transparent objects need check alpha value from Albedo Texture
			pipelineSpec.Layout = { GeoLayout, 5 };
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("PointShadowMap", true);
			pipelineSpec.BackfaceCulling = true;
			pipelineSpec.DepthTest = true;
			pipelineSpec.Topology = PrimitiveTopology_Triangles;
			pipelineSpec.DepthOperator = DepthCompareOperator_Less;

			RenderPassSpecification renderPassSpec;
			Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
			RenderPass_Create(s_Data.PointShadowMapPass, renderPassSpec);
		}
	}
	// Deferred Lighting Pass
	{
		PipelineSpecification pipelineSpec;
		{
			FramebufferSpecification spec;

			// TODO: Implement Framebuffer
			Framebuffer_Create(pipelineSpec.TargetFramebuffer, spec);
		}
		{
			pipelineSpec.Layout = { FullScreenQuadLayout, 2 };
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("DeferredLighting", true);
			pipelineSpec.BackfaceCulling = false;
			pipelineSpec.DepthTest = false;
			pipelineSpec.Topology = PrimitiveTopology_Triangles;

			RenderPassSpecification renderPassSpec;
			Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
			RenderPass_Create(s_Data.DeferredLightingPass, renderPassSpec);
		}
	}
	// Composite Pass
	{
		PipelineSpecification pipelineSpec;
		{
			FramebufferSpecification spec;

			// TODO: Implement Framebuffer
			Framebuffer_Create(pipelineSpec.TargetFramebuffer, spec);
		}
		{
			pipelineSpec.Layout = { FullScreenQuadLayout, 2 };
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("Composite", true);
			pipelineSpec.BackfaceCulling = false;
			pipelineSpec.DepthTest = false;
			pipelineSpec.Topology = PrimitiveTopology_Triangles;

			RenderPassSpecification renderPassSpec;
			Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
			RenderPass_Create(s_Data.CompositePass, renderPassSpec);
		}
	}
	// Skybox Pass
	{
		PipelineSpecification pipelineSpec;
		{
			FramebufferSpecification spec;

			// TODO: Implement Framebuffer
			Framebuffer_Create(pipelineSpec.TargetFramebuffer, spec);
		}
		{
			pipelineSpec.Layout = { SkyboxLayout, 1 };
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("Skybox", true);
			pipelineSpec.BackfaceCulling = false;
			pipelineSpec.DepthTest = true;
			pipelineSpec.Topology = PrimitiveTopology_Triangles;
			pipelineSpec.DepthOperator = DepthCompareOperator_LessEqual;

			RenderPassSpecification renderPassSpec;
			Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
			RenderPass_Create(s_Data.SkyboxPass, renderPassSpec);
		}
	}

	RenderPass_SetInput(s_Data.DeferredGeoPass, "CBModel", RendererResourceType_ConstantBuffer, (void*)&s_Data.ModelDataBuffer);
	RenderPass_SetInput(s_Data.DeferredGeoPass, "CBCamera", RendererResourceType_ConstantBuffer, (void*)&s_Data.CameraDataBuffer);

	RenderPass_SetInput(s_Data.DirShadowMapPass, "CBModel", RendererResourceType_ConstantBuffer, (void*)&s_Data.ModelDataBuffer);
	RenderPass_SetInput(s_Data.DirShadowMapPass, "CBDirShadow", RendererResourceType_ConstantBuffer, (void*)&s_Data.DirShadowDataBuffer);

	RenderPass_SetInput(s_Data.PointShadowMapPass, "CBModel", RendererResourceType_ConstantBuffer, (void*)&s_Data.ModelDataBuffer);
	RenderPass_SetInput(s_Data.PointShadowMapPass, "CBPointShadow", RendererResourceType_ConstantBuffer, (void*)&s_Data.PointShadowDataBuffer);

	RenderPass_SetInput(s_Data.DeferredLightingPass, "CBCamera", RendererResourceType_ConstantBuffer, (void*)&s_Data.CameraDataBuffer);
	RenderPass_SetInput(s_Data.DeferredLightingPass, "CBScene", RendererResourceType_ConstantBuffer, (void*)&s_Data.SceneDataBuffer);
	RenderPass_SetInput(s_Data.DeferredLightingPass, "CBPointLight", RendererResourceType_ConstantBuffer, (void*)&s_Data.PointLightDataBuffer);
	RenderPass_SetInput(s_Data.DeferredLightingPass, "CBSpotLight", RendererResourceType_ConstantBuffer, (void*)&s_Data.SpotLightDataBuffer);
	RenderPass_SetInput(s_Data.DeferredLightingPass, "CBDirShadow", RendererResourceType_ConstantBuffer, (void*)&s_Data.DirShadowDataBuffer);
	//RenderPass_SetInput(s_Data.DeferredLightingPass, "u_AlbedoBuffer", RendererResourceType_Texture2D, (void*)s_Data.DeferredGeoPass->GetOutput(0));
	//RenderPass_SetInput(s_Data.DeferredLightingPass, "u_MREBuffer", RendererResourceType_Texture2D, (void*)s_Data.DeferredGeoPass->GetOutput(1));
	//RenderPass_SetInput(s_Data.DeferredLightingPass, "u_NormalBuffer", RendererResourceType_Texture2D, (void*)s_Data.DeferredGeoPass->GetOutput(2));
	//RenderPass_SetInput(s_Data.DeferredLightingPass, "u_PositionBuffer", RendererResourceType_Texture2D, (void*)s_Data.DeferredGeoPass->GetOutput(3));

	//RenderPass_SetInput(s_Data.DeferredLightingPass, "u_DirShadowMap", RendererResourceType_Texture2D, (void*)s_Data.DirShadowMapPass->GetDepthOutput());
	//RenderPass_SetInput(s_Data.DeferredLightingPass, "u_PointShadowMap", RendererResourceType_Texture2D, (void*)s_Data.PointShadowMapPass->GetDepthOutput());
	//RenderPass_SetInput(s_Data.DeferredLightingPass, "u_BRDFLUTTex", RendererResourceType_Texture2D, (void*)Renderer::GetTexture<Texture2D>("BRDFLut"));

	//RenderPass_SetInput(s_Data.CompositePass, "u_Color", RendererResourceType_Texture2D, (void*)s_Data.DeferredLightingPass->GetOutput());

	RenderPass_SetInput(s_Data.SkyboxPass, "CBCamera", RendererResourceType_ConstantBuffer, (void*)&s_Data.CameraDataBuffer);
}

void SceneRenderer_Shutdown()
{
	ConstantBuffer_Release(s_Data.ModelDataBuffer);
	ConstantBuffer_Release(s_Data.CameraDataBuffer);
	ConstantBuffer_Release(s_Data.SceneDataBuffer);
	ConstantBuffer_Release(s_Data.PointLightDataBuffer);
	ConstantBuffer_Release(s_Data.SpotLightDataBuffer);
	ConstantBuffer_Release(s_Data.DirShadowDataBuffer);
	ConstantBuffer_Release(s_Data.PointShadowDataBuffer);

	RenderPass_Release(s_Data.DeferredGeoPass);
	RenderPass_Release(s_Data.DirShadowMapPass);
	RenderPass_Release(s_Data.PointShadowMapPass);
	RenderPass_Release(s_Data.DeferredLightingPass);
	RenderPass_Release(s_Data.CompositePass);
	RenderPass_Release(s_Data.SkyboxPass);

	//Material_Release(s_Data.DefaultMaterial);
}

void SceneRenderer_SetViewportSize(uint32_t width, uint32_t height)
{

}

void SceneRenderer_BeginScene(const Environment& environment)
{

}

void SceneRenderer_EndScene()
{

}

void ScnenRenderer_SubmitStaticMesh(const Mat4& transform, MeshComponent& meshComponent, Material* overrideMaterial)
{

}

RenderPass* SceneRenderer_GetFinalPass()
{
	return nullptr;
}

Image2D* SceneRenderer_GetGBufferAlbedo()
{
	return nullptr;
}

Image2D* SceneRenderer_GetGBufferMRE()
{
	return nullptr;
}

Image2D* SceneRenderer_GetGBufferNormal()
{
	return nullptr;
}

Image2D* SceneRenderer_GetGBufferPosition()
{
	return nullptr;
}

Image2D* SceneRenderer_GetDirShadowMap()
{
	return nullptr;
}

void SceneRenderer_ResetStats()
{

}

SceneRendererStatistics SceneRenderer_GetStats()
{
	return SceneRendererStatistics();
}