#pragma once
#include "Core.h"
#include "Camera/CameraController.h"
#include "Player/PlayerController.h"

static const char* s_SkyBoxHDRTexPath = "assets/textures/envmap/rustig_koppie_puresky_4k.hdr";

// Debug
static const char* s_AlbedoTexPath = "assets/textures/used-stainless-steel2-ue/used-stainless-steel2_albedo.png";
static const char* s_NormalTexPath = "assets/textures/used-stainless-steel2-ue/used-stainless-steel2_normal-dx.png";
static const char* s_MetallicRoughnessTexPath = "assets/textures/used-stainless-steel2-ue/used-stainless-steel2_roughness.png";

// Hardcoded scene
void ScriptGlue_CreateTestScene(Scene& scene)
{
	Material PBRTestMaterial;
	Material_Create(PBRTestMaterial);
	Material_SetAlbedoMap(PBRTestMaterial, (Texture2D*)AssetManager_GetAsset(s_AlbedoTexPath));
	Material_SetNormalMap(PBRTestMaterial, (Texture2D*)AssetManager_GetAsset(s_NormalTexPath));
	Material_SetMetallicRoughnessMap(PBRTestMaterial, (Texture2D*)AssetManager_GetAsset(s_MetallicRoughnessTexPath));
	AssetManager_GetAsset("PBRTestMaterial", AssetType_Material, &PBRTestMaterial);

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
		skyLight.Intensity = 1.0f;
		skyLight.EnvMap = (EnvMap*)AssetManager_GetAsset(s_SkyBoxHDRTexPath);
		Entity_AddComponent(&testObj, ComponentType::ComponentType_SkyLight, &skyLight);

		Scene_AddEntity(scene, testObj);
	}
	{
		Entity testObj = {};
		testObj.Tag.Name = "Box-1";
		testObj.Transform.Translation = { 0.0f, 0.0f, 0.0f };
		testObj.Transform.Rotation = { 0, 0, 0 };
		testObj.Transform.Scale = { 1.0f, 1.0f, 1.0f };

		MeshComponent mesh = {};
		mesh.Mesh = (Mesh*)AssetManager_GetAsset("Box");
		mesh.Material = (Material*)AssetManager_GetAsset("PBRTestMaterial");
		Entity_AddComponent(&testObj, ComponentType::ComponentType_Mesh, &mesh);

		Scene_AddEntity(scene, testObj);
	}
	{
		Entity testObj = {};
		testObj.Tag.Name = "Box-2";
		testObj.Transform.Translation = { 5.0f, 0.0f, 0.0f };
		testObj.Transform.Rotation = { 0, 0, 0 };
		testObj.Transform.Scale = { 1.0f, 1.0f, 1.0f };

		MeshComponent mesh = {};
		mesh.Mesh = (Mesh*)AssetManager_GetAsset("Box");
		mesh.Material = (Material*)AssetManager_GetAsset("PBRTestMaterial");
		Entity_AddComponent(&testObj, ComponentType::ComponentType_Mesh, &mesh);

		Scene_AddEntity(scene, testObj);
	}
	{
		Entity testObj = {};
		testObj.Tag.Name = "Box-3";
		testObj.Transform.Translation = { 10.0f, 0.0f, 0.0f };
		testObj.Transform.Rotation = { 0, 0, 0 };
		testObj.Transform.Scale = { 1.0f, 1.0f, 1.0f };

		MeshComponent mesh = {};
		mesh.Mesh = (Mesh*)AssetManager_GetAsset("Box");
		mesh.Material = (Material*)AssetManager_GetAsset("PBRTestMaterial");
		Entity_AddComponent(&testObj, ComponentType::ComponentType_Mesh, &mesh);

		Scene_AddEntity(scene, testObj);
	}
}