#pragma once
#include "Core.h"
#include "Camera/CameraController.h"
#include <stdio.h>

static const char* s_SkyBoxHDRTexPath = "assets/textures/envmap/rustig_koppie_puresky_4k.hdr";
static const char* s_CubePath = "assets/models/Cube.gltf";
static const char* s_SpherePath = "assets/models/Sphere.gltf";

// Debug
static const char* s_AlbedoTexPath = "assets/textures/used-stainless-steel2-ue/used-stainless-steel2_albedo.png";
static const char* s_NormalTexPath = "assets/textures/used-stainless-steel2-ue/used-stainless-steel2_normal-dx.png";
static const char* s_MetallicRoughnessTexPath = "assets/textures/used-stainless-steel2-ue/used-stainless-steel2_roughness.png";

// Load all assets, whatever it used in the title screen or game
// But no need to free them, AssetManager will handle it
void ScriptGlue_LoadAssets()
{
	AssetManager_GetAsset(s_CubePath);
	AssetManager_GetAsset(s_SkyBoxHDRTexPath);
	AssetManager_GetAsset(s_SpherePath);

	// Debug
	Material PBRTestMaterial;
	Material_Create(&PBRTestMaterial);
	Material_SetAlbedoMap(&PBRTestMaterial, (Texture2D*)AssetManager_GetAsset(s_AlbedoTexPath));
	Material_SetNormalMap(&PBRTestMaterial, (Texture2D*)AssetManager_GetAsset(s_NormalTexPath));
	Material_SetMetallicRoughnessMap(&PBRTestMaterial, (Texture2D*)AssetManager_GetAsset(s_MetallicRoughnessTexPath));
	Material_SetAlbedo(&PBRTestMaterial, { 1.0f, 1.0f, 1.0f });
	Material_SetEmission(&PBRTestMaterial, 0.0f);
	Material_SetMetallic(&PBRTestMaterial, 0.0f);
	Material_SetRoughness(&PBRTestMaterial, 1.0f);
	Material_SetUseNormalMap(&PBRTestMaterial, true);
	AssetManager_GetAsset("PBRTestMaterial", AssetType_Material, &PBRTestMaterial);
}

// Hardcoded scene
void ScriptGlue_CreateTestScene(Scene& scene)
{
	//Main Camera
	{
		Entity camera = {};
		camera.Tag.Name = "MainCamera";
		camera.Transform.Translation = { 0.0f, 0.0f, 10.0f };

		CameraSpecification spec = {};
		spec.ProjectionType = ProjectionType::Perspective;
		spec.AspectRatio = 16.0f / 9.0f;
		spec.PerspectiveFOV = DirectX::XMConvertToRadians(60.0f);
		spec.PerspectiveFar = 1000.0f;
		spec.PerspectiveNear = 0.01f;

		SceneCamera sceneCamera = {};
		SceneCamera_Create(sceneCamera, spec);

		CameraComponent cameraComponent = {};
		cameraComponent.Camera = sceneCamera;
		Entity_AddComponent(&camera, ComponentType::ComponentType_Camera, &cameraComponent);

		ScriptComponent scriptComponent = {};
		scriptComponent.OnCreate = CameraController_OnCreate;
		scriptComponent.OnUpdate = CameraController_OnUpdate;
		scriptComponent.OnDestroy = CameraController_OnDestroy;
		scriptComponent.OnCollision = CameraController_OnCollision;
		scriptComponent.OnRaycastHit = CameraController_OnRaycastHit;
		scriptComponent.OnEnable = CameraController_OnEnable;
		scriptComponent.OnDisable = CameraController_OnDisable;
		Entity_AddComponent(&camera, ComponentType::ComponentType_Script, &scriptComponent);

		Scene_AddEntity(scene, camera);
	}
	{
		Entity testObj = {};
		testObj.Tag.Name = "Sky";
		testObj.Transform.Translation = { 0.0f, 0.0f, 0.0f };
		testObj.Transform.Rotation = { 0, 0, 0 };
		testObj.Transform.Scale = { 1.0f, 1.0f, 1.0f };

		SkyLightComponent skyLight = {};
		skyLight.Intensity = 0.2f;
		skyLight.EnvMap = (EnvMap*)AssetManager_GetAsset(s_SkyBoxHDRTexPath);
		Entity_AddComponent(&testObj, ComponentType::ComponentType_SkyLight, &skyLight);

		Scene_AddEntity(scene, testObj);
	}
	{
		Entity testObj = {};
		testObj.Tag.Name = "DirLight";
		testObj.Transform.Translation = { 0.0f, 0.0f, 0.0f };
		testObj.Transform.Rotation = { 0, 0, 0 };
		testObj.Transform.Scale = { 1.0f, 1.0f, 1.0f };

		LightComponent light = {};
		light.Type = LightComponent::LightType_Directional;
		light.Radiance = { 1.0f, 1.0f, 1.0f };
		light.Intensity = 1.0f;
		light.Shadow = LightComponent::ShadowType_Hard;
		Entity_AddComponent(&testObj, ComponentType::ComponentType_Light, &light);

		Scene_AddEntity(scene, testObj);
	}

	for (size_t i = 0; i < 100; i++)
	{
		for (size_t j = 0; j < 100; j++)
		{
			Entity testObj = {};
			char tempName[256];
			sprintf_s(tempName, 256, "Box-%d-%d", j, i);

			testObj.Tag.Name = tempName;
			testObj.Transform.Translation = { 0.0f, 0.0f, 0.0f };
			testObj.Transform.Translation.x = (float)j * 2.0f;
			testObj.Transform.Translation.y = (float)i * 2.0f;
			testObj.Transform.Rotation = { 0, 0, 0 };
			testObj.Transform.Scale = { 1.0f, 1.0f, 1.0f };

			MeshComponent mesh = {};
			mesh.Mesh = (Mesh*)AssetManager_GetAsset(s_CubePath);
			mesh.Material = (Material*)AssetManager_GetAsset("PBRTestMaterial");
			Entity_AddComponent(&testObj, ComponentType::ComponentType_Mesh, &mesh);

			Scene_AddEntity(scene, testObj);
		}
	}
}