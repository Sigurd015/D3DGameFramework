#pragma once
#include "Core.h"
#include "Camera/CameraController.h"
#include "Player/PlayerController.h"

// Hardcoded scene
void ScriptGlue_CreateTestScene(Scene& scene)
{
	//Main Camera
	{
		Entity camera = {};
		camera.Tag.Name = "MainCamera";
		camera.Transform.Translation = { 0.0f, 0.0f, 10.0f };

		CameraSpecification spec = {};
		spec.ProjectionType = ProjectionType::Orthographic;
		spec.AspectRatio = 16.0f / 9.0f;
		spec.OrthographicSize = 50.0f;
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
	// UI Test
	{
		Entity selection = {};
		selection.Tag.Name = "UI_Test";

		RectTransformComponent rectTransform = {};
		rectTransform.Position = { 100.0f, 900.0f };
		rectTransform.Rotation = DirectX::XMConvertToRadians(45);
		rectTransform.Size = { 100.0f, 150.0f };
		Entity_AddComponent(&selection, ComponentType::ComponentType_RectTransform, &rectTransform);

		SpriteRendererComponent spriteRenderer = {};
		spriteRenderer.Color = { 1.0f, 0.0f, 0.0f, 1.0f };
		Entity_AddComponent(&selection, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

		Scene_AddEntity(scene, selection);
	}
	{
		Entity player = {};
		player.Tag.Name = "Player";
		player.Transform.Translation = { 0.0f, 0.0f, 0.0f };
		player.Transform.Rotation = { 0, 0, 0 };
		player.Transform.Scale = { 1.0f, 1.0f, 1.0f };

		Rigidbody2DComponent rigidbody2D = {};
		rigidbody2D.Type = Rigidbody2D::BodyType::Dynamic;
		rigidbody2D.GravityScale = 1.0f;
		Entity_AddComponent(&player, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

		BoxCollider2DComponent boxCollider2D = {};
		boxCollider2D.Restitution = 0.0f;
		boxCollider2D.Offset = { 0,0 };
		boxCollider2D.Size = { 3.0f,3.0f };
		Entity_AddComponent(&player, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

		ScriptComponent scriptComponent = {};
		scriptComponent.OnCreate = PlayerController_OnCreate;
		scriptComponent.OnUpdate = PlayerController_OnUpdate;
		scriptComponent.OnDestroy = PlayerController_OnDestroy;
		scriptComponent.OnCollision = PlayerController_OnCollision;
		scriptComponent.OnRaycastHit = PlayerController_OnRaycastHit;
		scriptComponent.OnEnable = PlayerController_OnEnable;
		scriptComponent.OnDisable = PlayerController_OnDisable;
		Entity_AddComponent(&player, ComponentType::ComponentType_Script, &scriptComponent);

		Scene_AddEntity(scene, player);
	}
	{
		Entity testObj = {};
		testObj.Tag.Name = "TestQuad";
		testObj.Transform.Translation = { 0.0f, 0.0f, 0.0f };
		testObj.Transform.Rotation = { 0, 0, 0 };
		testObj.Transform.Scale = { 1.0f, 1.0f, 1.0f };

		Rigidbody2DComponent rigidbody2D = {};
		rigidbody2D.Type = Rigidbody2D::BodyType::Dynamic;
		rigidbody2D.GravityScale = 1.0f;
		Entity_AddComponent(&testObj, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

		BoxCollider2DComponent boxCollider2D = {};
		boxCollider2D.Restitution = 0.0f;
		boxCollider2D.Offset = { 0,0 };
		boxCollider2D.Size = { 3.0f,3.0f };
		Entity_AddComponent(&testObj, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

		Scene_AddEntity(scene, testObj);
	}
	{
		Entity testObj = {};
		testObj.Tag.Name = "TestCsircle";
		testObj.Transform.Translation = { 0.0f, 0.0f, 0.0f };
		testObj.Transform.Rotation = { 0, 0, 0 };
		testObj.Transform.Scale = { 1.0f, 1.0f, 1.0f };

		Rigidbody2DComponent rigidbody2D = {};
		rigidbody2D.Type = Rigidbody2D::BodyType::Dynamic;
		rigidbody2D.GravityScale = 1.0f;
		Entity_AddComponent(&testObj, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

		CircleCollider2DComponent circleCollider2D = {};
		circleCollider2D.Restitution = 0.0f;
		circleCollider2D.Offset = { 0,0 };
		circleCollider2D.Radius = 3.0f;
		Entity_AddComponent(&testObj, ComponentType::ComponentType_CircleCollider2D, &circleCollider2D);

		Scene_AddEntity(scene, testObj);
	}
	{
		Entity testObj = {};
		testObj.Tag.Name = "Ground";
		testObj.Transform.Translation = { 0.0f, -20.0f, 0.0f };
		testObj.Transform.Rotation = { 0, 0, 0 };
		testObj.Transform.Scale = { 100.0f, 1.0f, 1.0f };

		Rigidbody2DComponent rigidbody2D = {};
		rigidbody2D.Type = Rigidbody2D::BodyType::Static;
		Entity_AddComponent(&testObj, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

		BoxCollider2DComponent boxCollider2D = {};
		boxCollider2D.Restitution = 0.0f;
		boxCollider2D.Offset = { 0,0 };
		boxCollider2D.Size = { 1.0f,1.0f };
		Entity_AddComponent(&testObj, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

		Scene_AddEntity(scene, testObj);
	}
}