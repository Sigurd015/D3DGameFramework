#include "pch.h"
#include "SceneRenderer.h"
#include "Asset/AssetManager.h"
#include "Mesh.h"
#include "MeshFactory.h"
#include "Scene/Scene.h"
#include "RendererAPI.h"

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
		Mat Transform;
		MaterialData Material;
	};

	struct CBCamera
	{
		Mat ViewProj;
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
		Mat LightViewProj;
		uint32_t ShadowType = 0;

		// Padding
		char padding[12];
	};

	struct CBPointShadow
	{
		Mat LightViewProj[6];
		Vec3 LightPosition;
		float FarPlane = 0.0f;
		uint32_t ShadowType = 0;

		// Padding
		char padding[12];
	};

	Environment SceneEnvironment;

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

	Material DefaultMaterial;

	struct GeoDrawCommand
	{
		Mesh* Mesh;
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

	VertexBufferLayout GeoLayoutInfo = { GeoLayout, 5 };
	VertexBufferLayout_CalculateOffsetsAndStride(GeoLayoutInfo);

	VertexBufferLayoutEmelent FullScreenQuadLayout[2] = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" },
	};

	VertexBufferLayout FullScreenQuadLayoutInfo = { FullScreenQuadLayout, 2 };
	VertexBufferLayout_CalculateOffsetsAndStride(FullScreenQuadLayoutInfo);

	VertexBufferLayoutEmelent SkyboxLayout[1] = {
		{ ShaderDataType::Float3, "a_Position" },
	};

	VertexBufferLayout SkyboxLayoutInfo = { SkyboxLayout, 1 };
	VertexBufferLayout_CalculateOffsetsAndStride(SkyboxLayoutInfo);

	// Deferred Geometry Pass
	{
		PipelineSpecification pipelineSpec;
		{
			FramebufferSpecification spec;
			FramebufferTextureSpecification colorAttachmentSpec[5] = {
				{ImageFormat_RGBA32F},
				{ImageFormat_RGBA16F},
				{ImageFormat_RGBA16F},
				{ImageFormat_RGBA16F},
				{ImageFormat_Depth}
			};
			spec.Attachments = { colorAttachmentSpec, 5 };
			spec.Width = 1920;
			spec.Height = 1080;
			spec.SwapChainTarget = false;

			pipelineSpec.TargetFramebuffer = (Framebuffer*)RendererResourcePool_GetResource(RendererResourceType_FrameBuffer, &spec);
		}
		{
			pipelineSpec.Layout = GeoLayoutInfo;
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("DeferredGeometry", AssetType_Shader);
			pipelineSpec.BackfaceCulling = true;
			pipelineSpec.DepthTest = true;
			pipelineSpec.Topology = PrimitiveTopology_Triangles;
			pipelineSpec.DepthOperator = DepthCompareOperator_Less;

			RenderPassSpecification renderPassSpec;
			Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
			RenderPass_Create(s_Data.DeferredGeoPass, renderPassSpec);

			// Create default material
			Material_Create(s_Data.DefaultMaterial, pipelineSpec.Shader);
			Material_SetTexture(s_Data.DefaultMaterial, "u_AlbedoTex",
				(Texture2D*)AssetManager_GetAsset("WhiteTexture", AssetType_Texture));
			Material_SetTexture(s_Data.DefaultMaterial, "u_MetallicRoughnessTex",
				(Texture2D*)AssetManager_GetAsset("WhiteTexture", AssetType_Texture));
			Material_SetTexture(s_Data.DefaultMaterial, "u_NormalTex",
				(Texture2D*)AssetManager_GetAsset("WhiteTexture", AssetType_Texture));
		}
	}

	// Dir Shadow Map Pass
	{
		PipelineSpecification pipelineSpec;
		{
			FramebufferSpecification spec;
			FramebufferTextureSpecification colorAttachmentSpec = {
				ImageFormat_ShadowMap
			};
			spec.Attachments = { &colorAttachmentSpec,1 };
			spec.Width = 2048;
			spec.Height = 2048;
			spec.SwapChainTarget = false;
			spec.DepthClearValue = 1.0f;

			pipelineSpec.TargetFramebuffer = (Framebuffer*)RendererResourcePool_GetResource(RendererResourceType_FrameBuffer, &spec);
		}
		{
			// Notice: Use the same layout as DeferredGeoPass, 
			// because transparent objects need check alpha value from Albedo Texture
			pipelineSpec.Layout = GeoLayoutInfo;
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("DirShadowMap", AssetType_Shader);
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
			FramebufferTextureSpecification colorAttachmentSpec = {
				ImageFormat_ShadowMap, 6
			};
			spec.Attachments = { &colorAttachmentSpec,1 };
			spec.Width = 2048;
			spec.Height = 2048;
			spec.SwapChainTarget = false;
			spec.DepthClearValue = 1.0f;

			pipelineSpec.TargetFramebuffer = (Framebuffer*)RendererResourcePool_GetResource(RendererResourceType_FrameBuffer, &spec);
		}
		{
			// Notice: Use the same layout as DeferredGeoPass, 
			// because transparent objects need check alpha value from Albedo Texture
			pipelineSpec.Layout = GeoLayoutInfo;
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("PointShadowMap", AssetType_Shader);
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
			FramebufferTextureSpecification colorAttachmentSpec = {
				ImageFormat_RGBA32F
			};
			spec.Attachments = { &colorAttachmentSpec,1 };
			spec.Width = 1920;
			spec.Height = 1080;
			spec.SwapChainTarget = false;

			pipelineSpec.TargetFramebuffer = (Framebuffer*)RendererResourcePool_GetResource(RendererResourceType_FrameBuffer, &spec);
		}
		{
			pipelineSpec.Layout = FullScreenQuadLayoutInfo;
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("DeferredLighting", AssetType_Shader);
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
			FramebufferTextureSpecification colorAttachmentSpec = {
				ImageFormat_RGBA32F
			};
			spec.Attachments = { &colorAttachmentSpec,1 };
			spec.Width = 1920;
			spec.Height = 1080;
			spec.SwapChainTarget = false;

			pipelineSpec.TargetFramebuffer = (Framebuffer*)RendererResourcePool_GetResource(RendererResourceType_FrameBuffer, &spec);
		}
		{
			pipelineSpec.Layout = FullScreenQuadLayoutInfo;
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("Composite", AssetType_Shader);
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
			FramebufferTextureSpecification colorAttachmentSpec[2] = {
				{ImageFormat_RGBA32F},
				{ImageFormat_Depth}
			};
			spec.Attachments = { colorAttachmentSpec,2 };
			spec.Width = 1920;
			spec.Height = 1080;
			spec.SwapChainTarget = false;
			ExistingImage existingImage[2] = {
				{RenderPass_GetOutput(s_Data.CompositePass),0},
				{RenderPass_GetDepthOutput(s_Data.DeferredGeoPass),1}
			};
			spec.ExistingImages = { existingImage,2 };

			pipelineSpec.TargetFramebuffer = (Framebuffer*)RendererResourcePool_GetResource(RendererResourceType_FrameBuffer, &spec);
		}
		{
			pipelineSpec.Layout = SkyboxLayoutInfo;
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("Skybox", AssetType_Shader);
			pipelineSpec.BackfaceCulling = false;
			pipelineSpec.DepthTest = true;
			pipelineSpec.Topology = PrimitiveTopology_Triangles;
			pipelineSpec.DepthOperator = DepthCompareOperator_LessEqual;

			RenderPassSpecification renderPassSpec;
			Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
			RenderPass_Create(s_Data.SkyboxPass, renderPassSpec);
		}
	}

	RenderPass_SetInput(s_Data.DeferredGeoPass, "CBModel", RendererResourceType_ConstantBuffer, &s_Data.ModelDataBuffer);
	RenderPass_SetInput(s_Data.DeferredGeoPass, "CBCamera", RendererResourceType_ConstantBuffer, &s_Data.CameraDataBuffer);

	RenderPass_SetInput(s_Data.DirShadowMapPass, "CBModel", RendererResourceType_ConstantBuffer, &s_Data.ModelDataBuffer);
	RenderPass_SetInput(s_Data.DirShadowMapPass, "CBDirShadow", RendererResourceType_ConstantBuffer, &s_Data.DirShadowDataBuffer);

	RenderPass_SetInput(s_Data.PointShadowMapPass, "CBModel", RendererResourceType_ConstantBuffer, &s_Data.ModelDataBuffer);
	RenderPass_SetInput(s_Data.PointShadowMapPass, "CBPointShadow", RendererResourceType_ConstantBuffer, &s_Data.PointShadowDataBuffer);

	RenderPass_SetInput(s_Data.DeferredLightingPass, "CBCamera", RendererResourceType_ConstantBuffer, &s_Data.CameraDataBuffer);
	RenderPass_SetInput(s_Data.DeferredLightingPass, "CBScene", RendererResourceType_ConstantBuffer, &s_Data.SceneDataBuffer);
	RenderPass_SetInput(s_Data.DeferredLightingPass, "CBPointLight", RendererResourceType_ConstantBuffer, &s_Data.PointLightDataBuffer);
	RenderPass_SetInput(s_Data.DeferredLightingPass, "CBSpotLight", RendererResourceType_ConstantBuffer, &s_Data.SpotLightDataBuffer);
	RenderPass_SetInput(s_Data.DeferredLightingPass, "CBDirShadow", RendererResourceType_ConstantBuffer, &s_Data.DirShadowDataBuffer);
	RenderPass_SetInput(s_Data.DeferredLightingPass, "u_AlbedoBuffer", RendererResourceType_Texture2D, RenderPass_GetOutput(s_Data.DeferredGeoPass, 0));
	RenderPass_SetInput(s_Data.DeferredLightingPass, "u_MREBuffer", RendererResourceType_Texture2D, RenderPass_GetOutput(s_Data.DeferredGeoPass, 1));
	RenderPass_SetInput(s_Data.DeferredLightingPass, "u_NormalBuffer", RendererResourceType_Texture2D, RenderPass_GetOutput(s_Data.DeferredGeoPass, 2));
	RenderPass_SetInput(s_Data.DeferredLightingPass, "u_PositionBuffer", RendererResourceType_Texture2D, RenderPass_GetOutput(s_Data.DeferredGeoPass, 3));

	RenderPass_SetInput(s_Data.DeferredLightingPass, "u_DirShadowMap", RendererResourceType_Texture2D, RenderPass_GetDepthOutput(s_Data.DirShadowMapPass));
	RenderPass_SetInput(s_Data.DeferredLightingPass, "u_PointShadowMap", RendererResourceType_Texture2D, RenderPass_GetDepthOutput(s_Data.PointShadowMapPass));
	// TODO: Better implementation for brdf lut (Directly use asset manager to get asset will generate mipmaps which is not needed)
	RenderPass_SetInput(s_Data.DeferredLightingPass, "u_BRDFLUTTex", RendererResourceType_Texture2D, AssetManager_GetAsset("assets/renderer/BRDF_LUT.png"));

	RenderPass_SetInput(s_Data.CompositePass, "u_Color", RendererResourceType_Texture2D, RenderPass_GetOutput(s_Data.DeferredLightingPass));

	RenderPass_SetInput(s_Data.SkyboxPass, "CBCamera", RendererResourceType_ConstantBuffer, &s_Data.CameraDataBuffer);


	TextureCreationOptionalData optionalData;
	optionalData.Spec.Format = ImageFormat_RGBA;
	optionalData.Spec.Width = 1;
	optionalData.Spec.Height = 1;
	optionalData.Spec.GenerateMips = false;
	constexpr uint32_t blackCubeTextureData[6] = { 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000 };
	Buffer_Create(optionalData.TextureData, sizeof(blackCubeTextureData), blackCubeTextureData);

	// Forget the return value, just make sure the asset is loaded, may use it in the anthor place
	AssetManager_GetAsset("BlackCube", AssetType_TextureCube, &optionalData);

	MeshSource meshSource = MeshFactory_CreateBox({ 1.0f,1.0f,1.0f });
	AssetManager_GetAsset("Box", AssetType_Mesh, &meshSource);
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

	Material_Release(s_Data.DefaultMaterial);
}

void SceneRenderer_SetViewportSize(uint32_t width, uint32_t height)
{
	// TODO: Implement this (Resize Framebuffer)
}

void SceneRenderer_BeginScene(const Environment& environment)
{
	SceneRenderer_ResetStats();

	s_Data.CameraData.ViewProj = environment.ViewProjection;
	s_Data.CameraData.CameraPosition = environment.CameraPosition;
	ConstantBuffer_SetData(s_Data.CameraDataBuffer, &s_Data.CameraData);

	s_Data.SceneEnvironment = environment;

	// Sky Light
	{
		s_Data.SceneData.SkyLightIntensity = environment.SkyLightIntensity;

		if (environment.EnvMap)
		{
			RenderPass_SetInput(s_Data.SkyboxPass, "u_RadianceMap", RendererResourceType_TextureCube, &s_Data.SceneEnvironment.EnvMap->RadianceMap);
			RenderPass_SetInput(s_Data.DeferredLightingPass, "u_EnvRadianceTex", RendererResourceType_TextureCube, &s_Data.SceneEnvironment.EnvMap->RadianceMap);
			RenderPass_SetInput(s_Data.DeferredLightingPass, "u_EnvIrradianceTex", RendererResourceType_TextureCube, &s_Data.SceneEnvironment.EnvMap->IrradianceMap);
		}
		else
		{
			RenderPass_SetInput(s_Data.SkyboxPass, "u_RadianceMap", RendererResourceType_TextureCube, AssetManager_GetAsset("BlackCube"));
			RenderPass_SetInput(s_Data.DeferredLightingPass, "u_EnvRadianceTex", RendererResourceType_TextureCube, AssetManager_GetAsset("BlackCube"));
			RenderPass_SetInput(s_Data.DeferredLightingPass, "u_EnvIrradianceTex", RendererResourceType_TextureCube, AssetManager_GetAsset("BlackCube"));
		}
	}

	// Directional Light
	{
		s_Data.SceneData.Light.Direction = environment.DirLight.Direction;
		s_Data.SceneData.Light.Intensity = environment.DirLight.Intensity;
		s_Data.SceneData.Light.Radiance = environment.DirLight.Radiance;
		ConstantBuffer_SetData(s_Data.SceneDataBuffer, &s_Data.SceneData);
	}

	// Point Lights
	{
		s_Data.PointLightData.Count = List_Size(environment.PointLights);
		for (uint32_t i = 0; i < s_Data.PointLightData.Count; i++)
		{
			PointLight* pointLight = (PointLight*)List_Get(environment.PointLights, i);
			s_Data.PointLightData.PointLights[i] = {
				pointLight->Position,
				pointLight->Intensity,
				pointLight->Radiance,
				pointLight->Radius,
				pointLight->Falloff,
			};
		}
		ConstantBuffer_SetData(s_Data.PointLightDataBuffer, &s_Data.PointLightData);
	}

	// Spot Lights
	{
		s_Data.SpotLightData.Count = List_Size(environment.SpotLights);
		for (uint32_t i = 0; i < s_Data.SpotLightData.Count; i++)
		{
			SpotLight* spotLight = (SpotLight*)List_Get(environment.SpotLights, i);
			s_Data.SpotLightData.SpotLights[i] = {
				spotLight->Position,
				spotLight->Intensity,
				spotLight->Radiance,
				spotLight->AngleAttenuation,
				spotLight->Direction,
				spotLight->Range,
				spotLight->Angle,
				spotLight->Falloff,
			};
		}
		ConstantBuffer_SetData(s_Data.SpotLightDataBuffer, &s_Data.SpotLightData);
	}

	List_Clear(s_Data.DrawCommands);
}

void ExecuteDrawCommands()
{
	List_Foreach(s_Data.DrawCommands, [](void* command) {
		SceneRendererData::GeoDrawCommand* drawCommand = (SceneRendererData::GeoDrawCommand*)command;

		s_Data.ModelData.Transform = drawCommand->ModelData.Transform;
		s_Data.ModelData.Material = drawCommand->ModelData.Material;

		ConstantBuffer_SetData(s_Data.ModelDataBuffer, &s_Data.ModelData);

		s_Data.RendererStats.DrawCalls++;

		RendererAPI_DrawMesh(drawCommand->Mesh, drawCommand->Material);
		});
}

void DeferredGeoPass()
{
	RendererAPI_BeginRenderPass(s_Data.DeferredGeoPass);
	ExecuteDrawCommands();
	RendererAPI_EndRenderPass();
}

void ShadowMapPass()
{
	// Directional Light
	{
		RendererAPI_BeginRenderPass(s_Data.DirShadowMapPass);
		s_Data.DirShadowData.ShadowType = (uint32_t)s_Data.SceneEnvironment.DirLight.ShadowType;
		if (s_Data.SceneEnvironment.DirLight.ShadowType != LightComponent::ShadowType_None)
		{
			const static Vec3 Zero = Vec3Zero;
			const static Vec3 Up = { 0.0f,1.0f,0.0f };
			Vec3 lightPosition = Vec3Sub(Zero, Vec3MulFloat(s_Data.SceneEnvironment.DirLight.Direction, 200.0f));
			Mat lightViewMatrix = DirectX::XMMatrixLookAtRH(DirectX::XMLoadFloat3(&lightPosition),
				DirectX::XMLoadFloat3(&Zero), DirectX::XMLoadFloat3(&Up));
			Mat lightOrthoMatrix = DirectX::XMMatrixOrthographicOffCenterRH(-350.0f, 350.0f, -350.0f, 350.0f, 0.1f, 750.0f);
			s_Data.DirShadowData.LightViewProj = lightOrthoMatrix * lightViewMatrix;

			ConstantBuffer_SetData(s_Data.DirShadowDataBuffer, &s_Data.DirShadowData);
			ExecuteDrawCommands();
		}
		else
		{
			ConstantBuffer_SetData(s_Data.DirShadowDataBuffer, &s_Data.DirShadowData);
		}
		RendererAPI_EndRenderPass();
	}
}

void DeferredLightPass()
{
	RendererAPI_BeginRenderPass(s_Data.DeferredLightingPass);
	RendererAPI_DrawFullScreenQuad();
	RendererAPI_EndRenderPass();
}

void CompositePass()
{
	RendererAPI_BeginRenderPass(s_Data.CompositePass);
	RendererAPI_DrawFullScreenQuad();
	RendererAPI_EndRenderPass();
}

void SkyboxPass()
{
	RendererAPI_BeginRenderPass(s_Data.SkyboxPass, false);
	RendererAPI_DrawMesh((Mesh*)AssetManager_GetAsset("Box"));
	RendererAPI_EndRenderPass();
}

void SceneRenderer_EndScene()
{
	DeferredGeoPass();

	ShadowMapPass();

	DeferredLightPass();

	CompositePass();
	SkyboxPass();
}

void ScnenRenderer_SubmitStaticMesh(const Mat& transform, MeshComponent* meshComponent)
{

}

const RenderPass& SceneRenderer_GetFinalPass()
{
	return s_Data.SkyboxPass;
}

Image2D* SceneRenderer_GetGBufferAlbedo()
{
	return RenderPass_GetOutput(s_Data.DeferredGeoPass, 0);
}

Image2D* SceneRenderer_GetGBufferMRE()
{
	return RenderPass_GetOutput(s_Data.DeferredGeoPass, 1);
}

Image2D* SceneRenderer_GetGBufferNormal()
{
	return RenderPass_GetOutput(s_Data.DeferredGeoPass, 2);
}

Image2D* SceneRenderer_GetGBufferPosition()
{
	return RenderPass_GetOutput(s_Data.DeferredGeoPass, 3);
}

Image2D* SceneRenderer_GetDirShadowMap()
{
	return RenderPass_GetDepthOutput(s_Data.DirShadowMapPass);
}

void SceneRenderer_ResetStats()
{
	s_Data.RendererStats = {};
}

SceneRendererStatistics SceneRenderer_GetStats()
{
	return s_Data.RendererStats;
}