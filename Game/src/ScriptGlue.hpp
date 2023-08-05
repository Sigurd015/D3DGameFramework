#pragma once
#include "Core.h"
#include "Player/PlayerController.h"
#include "Camera/CameraController.h"
#include "UI/TitleMenuController.h"
#include "UI/UIController.h"

// Hardcoded scene
void ScriptGlue_CreateTitleScene(Scene& scene)
{
	//Textures
	Texture2D* backgroundTex = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*backgroundTex, "assets/textures/title.png");

	//Main Camera
	{
		Entity* camera = (Entity*)malloc(sizeof(Entity));
		*camera = {};
		camera->Tag.Name = "MainCamera";
		camera->Transform.Translation = { 0.0f, 0.0f, -10.0f };

		CameraSpecification spec;
		spec.ProjectionType = ProjectionType::Orthographic;
		spec.AspectRatio = 16.0f / 9.0f;
		spec.OrthographicSize = 50.0f;
		SceneCamera* sceneCamera = (SceneCamera*)malloc(sizeof(SceneCamera));
		*sceneCamera = {};
		SceneCamera_Create(*sceneCamera, spec);

		CameraComponent* cameraComponent = (CameraComponent*)malloc(sizeof(CameraComponent));
		*cameraComponent = {};
		cameraComponent->Camera = sceneCamera;
		Entity_AddComponent(*camera, ComponentType::ComponentType_Camera, cameraComponent);

		Scene_AddEntity(scene, *camera);
	}

	//Background and Title menu manager
	{
		Entity* background = (Entity*)malloc(sizeof(Entity));
		*background = {};
		background->Tag.Name = "Background";

		RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
		*rectTransform = {};
		rectTransform->Position = { 0.0f, 0.0f };
		rectTransform->Size = { 1920.0f, 1080.0f };
		Entity_AddComponent(*background, ComponentType::ComponentType_RectTransform, rectTransform);

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = backgroundTex;
		spriteRenderer->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		spriteRenderer->UVStart = { 0,1.0f };
		spriteRenderer->UVEnd = { 1.0f,0 };
		Entity_AddComponent(*background, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		*scriptComponent = {};
		scriptComponent->OnCreate = TitleMenuController_OnCreate;
		scriptComponent->OnUpdate = TitleMenuController_OnUpdate;
		scriptComponent->OnDestroy = TitleMenuController_OnDestroy;
		scriptComponent->OnCollision = TitleMenuController_OnCollision;
		Entity_AddComponent(*background, ComponentType::ComponentType_Script, scriptComponent);

		Scene_AddEntity(scene, *background);
	}

	//Selection
	{
		Entity* selection = (Entity*)malloc(sizeof(Entity));
		*selection = {};
		selection->Tag.Name = "Selection";

		RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
		*rectTransform = {};
		rectTransform->Position = { 790.0f, 330.0f };
		rectTransform->Size = { 300.0f, 70.0f };
		Entity_AddComponent(*selection, ComponentType::ComponentType_RectTransform, rectTransform);

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Color = { 0.4f, 0.4f, 0.4f, 0.5f };
		Entity_AddComponent(*selection, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *selection);
	}
}

void ScriptGlue_CreatePlayScene(Scene& scene)
{
	//Textures
	Texture2D* wallTex1 = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*wallTex1, "assets/textures/1.png");

	Texture2D* wallTex2 = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*wallTex2, "assets/textures/2.png");

	Texture2D* wallTex3 = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*wallTex3, "assets/textures/3.png");

	Texture2D* wallTex4 = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*wallTex4, "assets/textures/4.png");

	Texture2D* wallTex5 = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*wallTex5, "assets/textures/5.png");

	//Main Camera
	{
		Entity* camera = (Entity*)malloc(sizeof(Entity));
		*camera = {};
		camera->Tag.Name = "MainCamera";
		camera->Transform.Translation = { 45.0f, -15.0f, 3.0f };
		camera->Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
		camera->Transform.Scale = { 1.0f, 1.0f, 1.0f };

		CameraSpecification spec;
		spec.ProjectionType = ProjectionType::Perspective;
		spec.AspectRatio = 16.0f / 9.0f;
		spec.PerspectiveFOV = DirectX::XMConvertToRadians(60.0f);
		spec.PerspectiveFar = 1000.0f;
		spec.PerspectiveNear = 0.01f;
		SceneCamera* sceneCamera = (SceneCamera*)malloc(sizeof(SceneCamera));
		*sceneCamera = {};
		SceneCamera_Create(*sceneCamera, spec);

		CameraComponent* cameraComponent = (CameraComponent*)malloc(sizeof(CameraComponent));
		*cameraComponent = {};
		cameraComponent->Camera = sceneCamera;
		cameraComponent->Primary = true;
		cameraComponent->FixedAspectRatio = false;
		Entity_AddComponent(*camera, ComponentType::ComponentType_Camera, cameraComponent);

		ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		*scriptComponent = {};
		scriptComponent->OnCreate = CameraController_OnCreate;
		scriptComponent->OnUpdate = CameraController_OnUpdate;
		scriptComponent->OnDestroy = CameraController_OnDestroy;
		scriptComponent->OnCollision = CameraController_OnCollision;
		Entity_AddComponent(*camera, ComponentType::ComponentType_Script, scriptComponent);

		Scene_AddEntity(scene, *camera);
	}

	{
		Entity* camera = (Entity*)malloc(sizeof(Entity));
		*camera = {};
		camera->Tag.Name = "DebugCamera";
		camera->Transform.Translation = { 0, 0, -60.0f };
		camera->Transform.Rotation = { 0, 0, 0 };
		camera->Transform.Scale = { 1.0f, 1.0f, 1.0f };

		CameraSpecification spec;
		spec.ProjectionType = ProjectionType::Perspective;
		spec.AspectRatio = 16.0f / 9.0f;
		spec.PerspectiveFOV = DirectX::XMConvertToRadians(60.0f);
		spec.PerspectiveFar = 1000.0f;
		spec.PerspectiveNear = 0.01f;
		SceneCamera* sceneCamera = (SceneCamera*)malloc(sizeof(SceneCamera));
		*sceneCamera = {};
		SceneCamera_Create(*sceneCamera, spec);

		CameraComponent* cameraComponent = (CameraComponent*)malloc(sizeof(CameraComponent));
		*cameraComponent = {};
		cameraComponent->Camera = sceneCamera;
		cameraComponent->Primary = false;
		cameraComponent->FixedAspectRatio = false;
		Entity_AddComponent(*camera, ComponentType::ComponentType_Camera, cameraComponent);

		Scene_AddEntity(scene, *camera);
	}

	{
		Entity* player = (Entity*)malloc(sizeof(Entity));
		*player = {};
		player->Tag.Name = "Player";
		player->Transform.Translation = { 45.0f, -15.0f, 0 };
		player->Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
		player->Transform.Scale = { 10.0f, 10.0f, 1.0f };

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		rigidbody2D->Type = Rigidbody2D::BodyType::Kinematic;
		Entity_AddComponent(*player, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		BoxCollider2DComponent* boxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		*boxCollider2D = {};
		boxCollider2D->Restitution = 0.0f;
		boxCollider2D->Offset = { 0,0 };
		boxCollider2D->Size = { 0.5f,0.1f };
		Entity_AddComponent(*player, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		*scriptComponent = {};
		scriptComponent->OnCreate = PlayerController_OnCreate;
		scriptComponent->OnUpdate = PlayerController_OnUpdate;
		scriptComponent->OnDestroy = PlayerController_OnDestroy;
		scriptComponent->OnCollision = PlayerController_OnCollision;
		Entity_AddComponent(*player, ComponentType::ComponentType_Script, scriptComponent);

		Scene_AddEntity(scene, *player);
	}

	//UI
	{
		{
			Entity* uiManager = (Entity*)malloc(sizeof(Entity));
			*uiManager = {};
			uiManager->Tag.Name = "UIManager";
			ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
			*scriptComponent = {};
			scriptComponent->OnCreate = UIController_OnCreate;
			scriptComponent->OnUpdate = UIController_OnUpdate;
			scriptComponent->OnDestroy = UIController_OnDestroy;
			scriptComponent->OnCollision = UIController_OnCollision;
			Entity_AddComponent(*uiManager, ComponentType::ComponentType_Script, scriptComponent);

			Scene_AddEntity(scene, *uiManager);
		}

		{
			Entity* hpBarFront = (Entity*)malloc(sizeof(Entity));
			*hpBarFront = {};
			hpBarFront->Tag.Name = "HPBarFront";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 150.0f, 990.0f };
			rectTransform->Size = { 500.0f, 25.0f };
			Entity_AddComponent(*hpBarFront, ComponentType::ComponentType_RectTransform, rectTransform);

			SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
			*spriteRenderer = {};
			spriteRenderer->Color = { 191.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f,0.8f };
			Entity_AddComponent(*hpBarFront, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

			Scene_AddEntity(scene, *hpBarFront);
		}

		{
			Entity* hpBarBack = (Entity*)malloc(sizeof(Entity));
			*hpBarBack = {};
			hpBarBack->Tag.Name = "HPBarBack";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 150.0f, 990.0f };
			rectTransform->Size = { 500.0f, 25.0f };
			Entity_AddComponent(*hpBarBack, ComponentType::ComponentType_RectTransform, rectTransform);

			SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
			*spriteRenderer = {};
			spriteRenderer->Color = { 235.0f / 255.0f, 94.0f / 255.0f, 94.0f / 255.0f,0.8f };
			Entity_AddComponent(*hpBarBack, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

			Scene_AddEntity(scene, *hpBarBack);
		}

		{
			Entity* hpBarBackground = (Entity*)malloc(sizeof(Entity));
			*hpBarBackground = {};
			hpBarBackground->Tag.Name = "HPBarBackground";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 150.0f, 990.0f };
			rectTransform->Size = { 500.0f, 25.0f };
			Entity_AddComponent(*hpBarBackground, ComponentType::ComponentType_RectTransform, rectTransform);

			SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
			*spriteRenderer = {};
			spriteRenderer->Color = { 64.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f,0.8f };
			Entity_AddComponent(*hpBarBackground, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

			Scene_AddEntity(scene, *hpBarBackground);
		}
	}

	{
		Entity* wall = (Entity*)malloc(sizeof(Entity));
		*wall = {};
		wall->Tag.Name = "wall-1";
		wall->Transform.Translation = { 15.0f,0,0 };
		wall->Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
		wall->Transform.Scale = { 10.0f,10.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = wallTex5;
		spriteRenderer->Color = { 1.0f,1.0f,1.0f,1.0f };
		Entity_AddComponent(*wall, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		rigidbody2D->Type = Rigidbody2D::BodyType::Static;
		Entity_AddComponent(*wall, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		BoxCollider2DComponent* boxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		*boxCollider2D = {};
		boxCollider2D->Restitution = 0.0f;
		boxCollider2D->Size = { 3.5f,0.1f };
		Entity_AddComponent(*wall, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		Scene_AddEntity(scene, *wall);
	}

	{
		Entity* wall = (Entity*)malloc(sizeof(Entity));
		*wall = {};
		wall->Tag.Name = "wall-2";
		wall->Transform.Translation = { 5.0f,0,0 };
		wall->Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
		wall->Transform.Scale = { 10.0f,10.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = wallTex2;
		Entity_AddComponent(*wall, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *wall);
	}

	{
		Entity* wall = (Entity*)malloc(sizeof(Entity));
		*wall = {};
		wall->Tag.Name = "wall-3";
		wall->Transform.Translation = { -5.0f,0,0 };
		wall->Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
		wall->Transform.Scale = { 10.0f,10.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = wallTex2;
		Entity_AddComponent(*wall, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *wall);
	}

	{
		Entity* wall = (Entity*)malloc(sizeof(Entity));
		*wall = {};
		wall->Tag.Name = "wall-4";
		wall->Transform.Translation = { -15.0f,0,0 };
		wall->Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
		wall->Transform.Scale = { 10.0f,10.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = wallTex2;
		Entity_AddComponent(*wall, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *wall);
	}

	{
		Entity* wall = (Entity*)malloc(sizeof(Entity));
		*wall = {};
		wall->Tag.Name = "wall-5";
		wall->Transform.Translation = { 25.0f,0,0 };
		wall->Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
		wall->Transform.Scale = { 10.0f,10.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = wallTex2;
		Entity_AddComponent(*wall, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *wall);
	}

	{
		Entity* wall = (Entity*)malloc(sizeof(Entity));
		*wall = {};
		wall->Tag.Name = "wall-6";
		wall->Transform.Translation = { 35.0f,0,0 };
		wall->Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
		wall->Transform.Scale = { 10.0f,10.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = wallTex2;
		Entity_AddComponent(*wall, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *wall);
	}

	{
		Entity* wall = (Entity*)malloc(sizeof(Entity));
		*wall = {};
		wall->Tag.Name = "wall-7";
		wall->Transform.Translation = { 45.0f,0,0 };
		wall->Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
		wall->Transform.Scale = { 10.0f,10.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = wallTex2;
		Entity_AddComponent(*wall, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *wall);
	}

	{
		Entity* wall = (Entity*)malloc(sizeof(Entity));
		*wall = {};
		wall->Tag.Name = "wall-8";
		wall->Transform.Translation = { 50.0f,-5.0f,0 };
		wall->Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
		wall->Transform.Scale = { 10.0f,10.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = wallTex2;
		spriteRenderer->UVStart = { 1.0f,1.0f };
		spriteRenderer->UVEnd = { 0.0f,0.0f };
		Entity_AddComponent(*wall, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *wall);
	}
	{
		Entity* wall = (Entity*)malloc(sizeof(Entity));
		*wall = {};
		wall->Tag.Name = "wall-9";
		wall->Transform.Translation = { 50.0f,-15.0f,0 };
		wall->Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
		wall->Transform.Scale = { 10.0f,10.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = wallTex2;
		spriteRenderer->UVStart = { 1.0f,1.0f };
		spriteRenderer->UVEnd = { 0.0f,0.0f };
		Entity_AddComponent(*wall, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		rigidbody2D->Type = Rigidbody2D::BodyType::Static;
		Entity_AddComponent(*wall, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		BoxCollider2DComponent* boxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		*boxCollider2D = {};
		boxCollider2D->Restitution = 0.0f;
		boxCollider2D->Size = { 1.5f,0.1f };
		Entity_AddComponent(*wall, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		Scene_AddEntity(scene, *wall);
	}

	{
		Entity* wall = (Entity*)malloc(sizeof(Entity));
		*wall = {};
		wall->Tag.Name = "wall-10";
		wall->Transform.Translation = { 50.0f,-25.0f,0 };
		wall->Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
		wall->Transform.Scale = { 10.0f,10.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = wallTex2;
		spriteRenderer->UVStart = { 1.0f,1.0f };
		spriteRenderer->UVEnd = { 0.0f,0.0f };
		Entity_AddComponent(*wall, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *wall);
	}

	{
		Entity* quad = (Entity*)malloc(sizeof(Entity));
		*quad = {};
		quad->Tag.Name = "Ground1";
		quad->Transform.Translation = { 15.0f,0,5.0f };
		quad->Transform.Scale = { 100.0f,500.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Color = { 0.5f,0.5f,0.5f,1.0f };
		Entity_AddComponent(*quad, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *quad);
	}
}

