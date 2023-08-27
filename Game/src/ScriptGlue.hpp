#pragma once
#include "Core.h"
#include "Player/PlayerController.h"
#include "Map/EnemyController.h"
#include "Camera/CameraController.h"
#include "UI/TitleMenuController.h"
#include "UI/UIController.h"
#include "Map/FieldController.h"
#include "Map/FieldTrigger.h"

// Hardcoded scene
void ScriptGlue_CreateTitleScene(Scene& scene)
{
	//Textures
	Texture2D* backgroundTex = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*backgroundTex, "assets/textures/title.png");

	//Main Camera
	{
		Entity camera = {};
		camera.Tag.Name = "MainCamera";
		camera.Transform.Translation = { 0.0f, 0.0f, -10.0f };

		CameraSpecification spec = {};
		spec.ProjectionType = ProjectionType::Orthographic;
		spec.AspectRatio = 16.0f / 9.0f;
		spec.OrthographicSize = 50.0f;
		SceneCamera sceneCamera = {};
		SceneCamera_Create(sceneCamera, spec);

		CameraComponent cameraComponent = {};
		cameraComponent.Camera = sceneCamera;
		Entity_AddComponent(camera, ComponentType::ComponentType_Camera, &cameraComponent);

		Scene_AddEntity(scene, camera);
	}

	//Background and Title menu manager
	{
		Entity background = {};
		background.Tag.Name = "Background";

		RectTransformComponent rectTransform = {};
		rectTransform.Position = { 0.0f, 0.0f };
		rectTransform.Size = { 1920.0f, 1080.0f };
		Entity_AddComponent(background, ComponentType::ComponentType_RectTransform, &rectTransform);

		SpriteRendererComponent spriteRenderer = {};
		spriteRenderer.Texture = backgroundTex;
		spriteRenderer.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Entity_AddComponent(background, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

		ScriptComponent scriptComponent = {};
		scriptComponent.OnCreate = TitleMenuController_OnCreate;
		scriptComponent.OnUpdate = TitleMenuController_OnUpdate;
		scriptComponent.OnDestroy = TitleMenuController_OnDestroy;
		scriptComponent.OnCollision = TitleMenuController_OnCollision;
		scriptComponent.OnRaycastHit = TitleMenuController_OnRaycastHit;
		scriptComponent.OnEnable = TitleMenuController_OnEnable;
		scriptComponent.OnDisable = TitleMenuController_OnDisable;
		Entity_AddComponent(background, ComponentType::ComponentType_Script, &scriptComponent);

		Scene_AddEntity(scene, background);
	}

	//Selection
	{
		Entity selection = {};
		selection.Tag.Name = "Selection";

		RectTransformComponent rectTransform = {};
		rectTransform.Position = { 790.0f, 330.0f };
		rectTransform.Size = { 300.0f, 70.0f };
		Entity_AddComponent(selection, ComponentType::ComponentType_RectTransform, &rectTransform);

		SpriteRendererComponent spriteRenderer = {};
		spriteRenderer.Color = { 0.4f, 0.4f, 0.4f, 0.5f };
		Entity_AddComponent(selection, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

		Scene_AddEntity(scene, selection);
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

	Texture2D* floorTex = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*floorTex, "assets/textures/floor.png");

	Texture2D* roofTex = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*roofTex, "assets/textures/roof.png");

	Texture2D* shotgunSprite = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*shotgunSprite, "assets/textures/weapon/shotgun/spritesheet.png");

	Texture2D* sightIconSprite = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*sightIconSprite, "assets/textures/sightIcon.png");

	Texture2D* cacoDemonWalkSprite = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*cacoDemonWalkSprite, "assets/textures/npc/caco_demon/walk/spritesheet.png");

	Texture2D* cacoDemonAttackSprite = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*cacoDemonAttackSprite, "assets/textures/npc/caco_demon/attack/spritesheet.png");

	Texture2D* cacoDemonDeathSprite = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*cacoDemonDeathSprite, "assets/textures/npc/caco_demon/death/spritesheet.png");

	Texture2D* cacoDemonPainSprite = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*cacoDemonPainSprite, "assets/textures/npc/caco_demon/pain/spritesheet.png");

	Texture2D* cyberDemonWalkSprite = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*cyberDemonWalkSprite, "assets/textures/npc/cyber_demon/walk/spritesheet.png");

	Texture2D* cyberDemonAttackSprite = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*cyberDemonAttackSprite, "assets/textures/npc/cyber_demon/attack/spritesheet.png");

	Texture2D* cyberDemonDeathSprite = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*cyberDemonDeathSprite, "assets/textures/npc/cyber_demon/death/spritesheet.png");

	Texture2D* cyberDemonPainSprite = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*cyberDemonPainSprite, "assets/textures/npc/cyber_demon/pain/spritesheet.png");

	//Main Camera
	{
		Entity camera = {};
		camera.Tag.Name = "MainCamera";
		camera.Transform.Translation = { 45.0f, -15.0f, 3.0f };
		camera.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
		camera.Transform.Scale = { 1.0f, 1.0f, 1.0f };

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
		cameraComponent.Primary = true;
		cameraComponent.FixedAspectRatio = false;
		Entity_AddComponent(camera, ComponentType::ComponentType_Camera, &cameraComponent);

		ScriptComponent scriptComponent = {};
		scriptComponent.OnCreate = CameraController_OnCreate;
		scriptComponent.OnUpdate = CameraController_OnUpdate;
		scriptComponent.OnDestroy = CameraController_OnDestroy;
		scriptComponent.OnCollision = CameraController_OnCollision;
		scriptComponent.OnRaycastHit = CameraController_OnRaycastHit;
		scriptComponent.OnEnable = CameraController_OnEnable;
		scriptComponent.OnDisable = CameraController_OnDisable;
		Entity_AddComponent(camera, ComponentType::ComponentType_Script, &scriptComponent);

		Scene_AddEntity(scene, camera);
	}

	#ifndef CORE_DIST
	{
		Entity camera;
		camera.Tag.Name = "DebugCamera";
		camera.Transform.Translation = { 0, 0, -60.0f };
		camera.Transform.Rotation = { 0, 0, 0 };
		camera.Transform.Scale = { 1.0f, 1.0f, 1.0f };

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
		cameraComponent.Primary = false;
		cameraComponent.FixedAspectRatio = false;
		Entity_AddComponent(camera, ComponentType::ComponentType_Camera, &cameraComponent);

		Scene_AddEntity(scene, camera);
	}
	#endif

	//Player
	{
		Entity player = {};
		player.Tag.Name = "Player";
		player.Transform.Translation = { 45.0f, -15.0f, 1.0f };
		player.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
		player.Transform.Scale = { 1.0f, 1.0f, 1.0f };

		Rigidbody2DComponent rigidbody2D = {};
		rigidbody2D.Type = Rigidbody2D::BodyType::Kinematic;
		rigidbody2D.GravityScale = 0.0f;
		Entity_AddComponent(player, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

		BoxCollider2DComponent boxCollider2D = {};
		boxCollider2D.Restitution = 0.0f;
		boxCollider2D.Offset = { 0,0 };
		boxCollider2D.Size = { 3.0f,1.0f };
		Entity_AddComponent(player, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

		ScriptComponent scriptComponent = {};
		scriptComponent.OnCreate = PlayerController_OnCreate;
		scriptComponent.OnUpdate = PlayerController_OnUpdate;
		scriptComponent.OnDestroy = PlayerController_OnDestroy;
		scriptComponent.OnCollision = PlayerController_OnCollision;
		scriptComponent.OnRaycastHit = PlayerController_OnRaycastHit;
		scriptComponent.OnEnable = PlayerController_OnEnable;
		scriptComponent.OnDisable = PlayerController_OnDisable;
		Entity_AddComponent(player, ComponentType::ComponentType_Script, &scriptComponent);

		Scene_AddEntity(scene, player);
	}

	//Enemy
	{
		Entity enemy = {};
		enemy.Tag.Name = "Enemy-1";
		enemy.Transform.Translation = { 45.0f, -5.0f, 1.5f };
		enemy.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
		enemy.Transform.Scale = { 5.0f, 5.0f, 1.0f };

		SpriteRendererComponent spriteRenderer = {};
		Entity_AddComponent(enemy, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

		Rigidbody2DComponent rigidbody2D = {};
		rigidbody2D.Type = Rigidbody2D::BodyType::Kinematic;
		Entity_AddComponent(enemy, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

		BoxCollider2DComponent boxCollider2D = {};
		boxCollider2D.Restitution = 0.0f;
		boxCollider2D.Offset = { 0,0 };
		boxCollider2D.Size = { 0.6f,0.2f };
		Entity_AddComponent(enemy, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

		ScriptComponent scriptComponent = {};
		scriptComponent.OnCreate = EnemyController_OnCreate;
		scriptComponent.OnUpdate = EnemyController_OnUpdate;
		scriptComponent.OnDestroy = EnemyController_OnDestroy;
		scriptComponent.OnCollision = EnemyController_OnCollision;
		scriptComponent.OnRaycastHit = EnemyController_OnRaycastHit;
		scriptComponent.OnEnable = EnemyController_OnEnable;
		scriptComponent.OnDisable = EnemyController_OnDisable;

		EnemyData* enemyData = (EnemyData*)malloc(sizeof(EnemyData));
		*enemyData = {};
		enemyData->Type = EnemyType::CACO_DEMON;
		enemyData->WalkSpriteSheet = cacoDemonWalkSprite;
		enemyData->AttackSpriteSheet = cacoDemonAttackSprite;
		enemyData->DeathSpriteSheet = cacoDemonDeathSprite;
		enemyData->PainSpriteSheet = cacoDemonPainSprite;

		//EnemyData* enemyData = (EnemyData*)malloc(sizeof(EnemyData));
		//*enemyData = {};
		//enemyData->Type = EnemyType::CYBER_DEMON;
		//enemyData->WalkSpriteSheet = cyberDemonWalkSprite;
		//enemyData->AttackSpriteSheet = cyberDemonAttackSprite;
		//enemyData->DeathSpriteSheet = cyberDemonDeathSprite;
		//enemyData->PainSpriteSheet = cyberDemonPainSprite;

		scriptComponent.RuntimeData = enemyData;

		Entity_AddComponent(enemy, ComponentType::ComponentType_Script, &scriptComponent);

		Scene_AddEntity(scene, enemy);
	}

	{
		Entity enemy = {};
		enemy.Tag.Name = "Enemy-2";
		enemy.Transform.Translation = { 35.0f, -5.0f, 1.5f };
		enemy.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
		enemy.Transform.Scale = { 5.0f, 5.0f, 1.0f };

		SpriteRendererComponent spriteRenderer = {};
		Entity_AddComponent(enemy, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

		Rigidbody2DComponent rigidbody2D = {};
		rigidbody2D.Type = Rigidbody2D::BodyType::Kinematic;
		Entity_AddComponent(enemy, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

		BoxCollider2DComponent boxCollider2D = {};
		boxCollider2D.Restitution = 0.0f;
		boxCollider2D.Offset = { 0,0 };
		boxCollider2D.Size = { 0.6f,0.2f };
		Entity_AddComponent(enemy, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

		ScriptComponent scriptComponent = {};
		scriptComponent.OnCreate = EnemyController_OnCreate;
		scriptComponent.OnUpdate = EnemyController_OnUpdate;
		scriptComponent.OnDestroy = EnemyController_OnDestroy;
		scriptComponent.OnCollision = EnemyController_OnCollision;
		scriptComponent.OnRaycastHit = EnemyController_OnRaycastHit;
		scriptComponent.OnEnable = EnemyController_OnEnable;
		scriptComponent.OnDisable = EnemyController_OnDisable;

		EnemyData* enemyData = (EnemyData*)malloc(sizeof(EnemyData));
		*enemyData = {};
		enemyData->Type = EnemyType::CACO_DEMON;
		enemyData->WalkSpriteSheet = cacoDemonWalkSprite;
		enemyData->AttackSpriteSheet = cacoDemonAttackSprite;
		enemyData->DeathSpriteSheet = cacoDemonDeathSprite;
		enemyData->PainSpriteSheet = cacoDemonPainSprite;

		//EnemyData* enemyData = (EnemyData*)malloc(sizeof(EnemyData));
		//*enemyData = {};
		//enemyData->Type = EnemyType::CYBER_DEMON;
		//enemyData->WalkSpriteSheet = cyberDemonWalkSprite;
		//enemyData->AttackSpriteSheet = cyberDemonAttackSprite;
		//enemyData->DeathSpriteSheet = cyberDemonDeathSprite;
		//enemyData->PainSpriteSheet = cyberDemonPainSprite;
		
		scriptComponent.RuntimeData = enemyData;

		Entity_AddComponent(enemy, ComponentType::ComponentType_Script, &scriptComponent);

		Scene_AddEntity(scene, enemy);
	}

	//UI
	{
		{
			Entity uiManager = {};
			uiManager.Tag.Name = "UIManager";
			ScriptComponent scriptComponent = {};
			scriptComponent.OnCreate = UIController_OnCreate;
			scriptComponent.OnUpdate = UIController_OnUpdate;
			scriptComponent.OnDestroy = UIController_OnDestroy;
			scriptComponent.OnCollision = UIController_OnCollision;
			scriptComponent.OnRaycastHit = UIController_OnRaycastHit;
			scriptComponent.OnEnable = UIController_OnEnable;
			scriptComponent.OnDisable = UIController_OnDisable;
			Entity_AddComponent(uiManager, ComponentType::ComponentType_Script, &scriptComponent);

			Scene_AddEntity(scene, uiManager);
		}
		{
			Entity weapon = {};
			weapon.Tag.Name = "Weapon";
			RectTransformComponent rectTransform;
			rectTransform.Position = { 695.0f, 0.0f };
			rectTransform.Size = { 500.0f, 500.0f };
			Entity_AddComponent(weapon, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Texture = shotgunSprite;
			Entity_AddComponent(weapon, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, weapon);
		}
		{
			Entity sightIcon = {};
			sightIcon.Tag.Name = "SightIcon";
			RectTransformComponent rectTransform = {};
			rectTransform.Position = { 935.0f, 515.0f };
			rectTransform.Size = { 50.0f, 50.0f };
			Entity_AddComponent(sightIcon, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Texture = sightIconSprite;
			Entity_AddComponent(sightIcon, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, sightIcon);
		}
		{
			Entity hpBarFront = {};
			hpBarFront.Tag.Name = "HPBarFront";
			RectTransformComponent rectTransform = {};
			rectTransform.Position = { 100.0f, 990.0f };
			rectTransform.Size = { 500.0f, 25.0f };
			Entity_AddComponent(hpBarFront, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Color = { 191.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f,0.8f };
			Entity_AddComponent(hpBarFront, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, hpBarFront);
		}
		{
			Entity hpBarBack = {};
			hpBarBack.Tag.Name = "HPBarBack";
			RectTransformComponent rectTransform = {};
			rectTransform.Position = { 100.0f, 990.0f };
			rectTransform.Size = { 500.0f, 25.0f };
			Entity_AddComponent(hpBarBack, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Color = { 235.0f / 255.0f, 94.0f / 255.0f, 94.0f / 255.0f,0.8f };
			Entity_AddComponent(hpBarBack, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, hpBarBack);
		}
		{
			Entity hpBarBackground = {};
			hpBarBackground.Tag.Name = "HPBarBackground";
			RectTransformComponent rectTransform = {};
			rectTransform.Position = { 100.0f, 990.0f };
			rectTransform.Size = { 500.0f, 25.0f };
			Entity_AddComponent(hpBarBackground, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Color = { 64.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f,0.8f };
			Entity_AddComponent(hpBarBackground, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, hpBarBackground);
		}
	}

	//Map
	{
		{
			Entity fieldManager = {};
			fieldManager.Tag.Name = "FieldManager";
			ScriptComponent scriptComponent = {};
			scriptComponent.OnCreate = FieldController_OnCreate;
			scriptComponent.OnUpdate = FieldController_OnUpdate;
			scriptComponent.OnDestroy = FieldController_OnDestroy;
			scriptComponent.OnCollision = FieldController_OnCollision;
			scriptComponent.OnRaycastHit = FieldController_OnRaycastHit;
			scriptComponent.OnEnable = FieldController_OnEnable;
			scriptComponent.OnDisable = FieldController_OnDisable;
			Entity_AddComponent(fieldManager, ComponentType::ComponentType_Script, &scriptComponent);

			Scene_AddEntity(scene, fieldManager);
		}

		{
			Entity wall = {};
			wall.Tag.Name = "back-wall";
			wall.Transform.Translation = { 15.0f,-30.0f,0 };
			wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
			wall.Transform.Scale = { 10.0f,10.0f,1.0f };

			Rigidbody2DComponent rigidbody2D = {};
			rigidbody2D.Type = Rigidbody2D::BodyType::Static;
			Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

			BoxCollider2DComponent boxCollider2D = {};
			boxCollider2D.Restitution = 0.0f;
			boxCollider2D.Size = { 3.5f,0.1f };
			Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

			Scene_AddEntity(scene, wall);
		}

		//Left
		{
			{
				Entity wall = {};
				wall.Tag.Name = "left-wall-1";
				wall.Transform.Translation = { 15.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex4;
				spriteRenderer.Color = { 1.0f,1.0f,1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "left-wall-2";
				wall.Transform.Translation = { 5.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "left-wall-3";
				wall.Transform.Translation = { -5.0f,31.5f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Offset = { 0.0f,2.0f };
				boxCollider2D.Size = { 0.5f,0.1f };
				boxCollider2D.IsTrigger = true;
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				ScriptComponent scriptComponent = {};
				scriptComponent.OnCreate = FieldTrigger_OnCreate;
				scriptComponent.OnUpdate = FieldTrigger_OnUpdate;
				scriptComponent.OnDestroy = FieldTrigger_OnDestroy;
				scriptComponent.OnCollision = FieldTrigger_OnCollision;
				scriptComponent.OnRaycastHit = FieldTrigger_OnRaycastHit;
				scriptComponent.OnEnable = FieldController_OnEnable;
				scriptComponent.OnDisable = FieldController_OnDisable;
				Entity_AddComponent(wall, ComponentType::ComponentType_Script, &scriptComponent);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "left-wall-4";
				wall.Transform.Translation = { -15.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 0.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "left-wall-5";
				wall.Transform.Translation = { 25.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "left-wall-6";
				wall.Transform.Translation = { 35.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 0.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "left-wall-7";
				wall.Transform.Translation = { 45.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 0.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "left-wall-8";
				wall.Transform.Translation = { 50.0f,25.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "left-wall-9";
				wall.Transform.Translation = { 50.0f,15.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "left-wall-10";
				wall.Transform.Translation = { 50.0f,5.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "left-wall-11";
				wall.Transform.Translation = { -20.0f,25.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "left-wall-12";
				wall.Transform.Translation = { -20.0f,15.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "left-wall-13";
				wall.Transform.Translation = { -20.0f,5.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex3;
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
		}

		//Right
		{
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-1";
				wall.Transform.Translation = { 15.0f,0,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex5;
				spriteRenderer.Color = { 1.0f,1.0f,1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-2";
				wall.Transform.Translation = { 5.0f,0,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-3";
				wall.Transform.Translation = { -5.0f,0,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 0.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-4";
				wall.Transform.Translation = { -15.0f,0,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 0.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-5";
				wall.Transform.Translation = { 25.0f,0,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-6";
				wall.Transform.Translation = { 35.0f,1.5f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Offset = { 0.0f,2.0f };
				boxCollider2D.Size = { 0.5f,0.1f };
				boxCollider2D.IsTrigger = true;
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				ScriptComponent scriptComponent = {};
				scriptComponent.OnCreate = FieldTrigger_OnCreate;
				scriptComponent.OnUpdate = FieldTrigger_OnUpdate;
				scriptComponent.OnDestroy = FieldTrigger_OnDestroy;
				scriptComponent.OnCollision = FieldTrigger_OnCollision;
				scriptComponent.OnRaycastHit = FieldTrigger_OnRaycastHit;
				Entity_AddComponent(wall, ComponentType::ComponentType_Script, &scriptComponent);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-7";
				wall.Transform.Translation = { 45.0f,0,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 0.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-8";
				wall.Transform.Translation = { 50.0f,-5.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-9";
				wall.Transform.Translation = { 50.0f,-15.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-10";
				wall.Transform.Translation = { 50.0f,-25.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-11";
				wall.Transform.Translation = { -20.0f,-5.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-12";
				wall.Transform.Translation = { -20.0f,-15.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "right-wall-13";
				wall.Transform.Translation = { -20.0f,-25.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
		}

		// End
		{
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-1";
				wall.Transform.Translation = { 15.0f,60.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex5;
				spriteRenderer.Color = { 1.0f,1.0f,1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 3.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-2";
				wall.Transform.Translation = { 5.0f,60.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-3";
				wall.Transform.Translation = { -5.0f,60.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-4";
				wall.Transform.Translation = { -15.0f,60.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-5";
				wall.Transform.Translation = { 25.0f,60.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-6";
				wall.Transform.Translation = { 35.0f,60.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-7";
				wall.Transform.Translation = { 45.0f,60.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-8";
				wall.Transform.Translation = { 50.0f,55.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-9";
				wall.Transform.Translation = { 50.0f,45.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-10";
				wall.Transform.Translation = { 50.0f,35.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-11";
				wall.Transform.Translation = { -20.0f,55.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-12";
				wall.Transform.Translation = { -20.0f,45.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.5f,0.1f };
				Entity_AddComponent(wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "end-wall-13";
				wall.Transform.Translation = { -20.0f,35.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = wallTex2;
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
		}
		{
			Entity quad = {};
			quad.Tag.Name = "Floor";
			quad.Transform.Translation = { 15.0f,0,5.0f };
			quad.Transform.Scale = { 100.0f,100.0f,1.0f };

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Texture = floorTex;
			spriteRenderer.TilingFactor = 5.0f;
			Entity_AddComponent(quad, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, quad);
		}
		{
			Entity quad = {};
			quad.Tag.Name = "Roof";
			quad.Transform.Translation = { 15.0f,0,-5.0f };
			quad.Transform.Rotation = { 0,DirectX::XMConvertToRadians(180.0f),0 };
			quad.Transform.Scale = { 100.0f,100.0f,1.0f };

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Texture = roofTex;
			spriteRenderer.TilingFactor = 5.0f;
			Entity_AddComponent(quad, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, quad);
		}
	}
}

