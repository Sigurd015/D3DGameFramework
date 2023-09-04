#pragma once
#include "Core.h"
#include "Player/PlayerController.h"
#include "Map/EnemyController.h"
#include "Camera/CameraController.h"
#include "UI/TitleMenuController.h"
#include "UI/UIController.h"
#include "Map/FieldController.h"
#include "Map/FieldTrigger.h"
#include "Map/LightController.h"
#include "Map/ItemTrigger.h"

static RefPtr* s_RedLight;
static RefPtr* s_GreenLight;

// Hardcoded scene
void ScriptGlue_CreateTitleScene(Scene& scene)
{
	Texture2D texture;
	Texture2D_Create(&texture, "assets/textures/title.png");
	RefPtr* refPtr = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/animated_sprites/red_light/spritesheet.png");
	s_RedLight = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/animated_sprites/green_light/spritesheet.png");
	s_GreenLight = RefPtr_Create(sizeof(Texture2D), &texture);

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
		Entity_AddComponent(&camera, ComponentType::ComponentType_Camera, &cameraComponent);

		Scene_AddEntity(scene, camera);
	}

	//Background and Title menu manager
	{
		Entity background = {};
		background.Tag.Name = "Background";

		RectTransformComponent rectTransform = {};
		rectTransform.Position = { 0.0f, 0.0f };
		rectTransform.Size = { 1920.0f, 1080.0f };
		Entity_AddComponent(&background, ComponentType::ComponentType_RectTransform, &rectTransform);

		SpriteRendererComponent spriteRenderer = {};
		spriteRenderer.Texture = RefPtr_AddRef(refPtr);
		spriteRenderer.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Entity_AddComponent(&background, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

		ScriptComponent scriptComponent = {};
		scriptComponent.OnCreate = TitleMenuController_OnCreate;
		scriptComponent.OnUpdate = TitleMenuController_OnUpdate;
		scriptComponent.OnDestroy = TitleMenuController_OnDestroy;
		scriptComponent.OnCollision = TitleMenuController_OnCollision;
		scriptComponent.OnRaycastHit = TitleMenuController_OnRaycastHit;
		scriptComponent.OnEnable = TitleMenuController_OnEnable;
		scriptComponent.OnDisable = TitleMenuController_OnDisable;
		Entity_AddComponent(&background, ComponentType::ComponentType_Script, &scriptComponent);

		Scene_AddEntity(scene, background);
	}

	//Selection
	{
		Entity selection = {};
		selection.Tag.Name = "Selection";

		RectTransformComponent rectTransform = {};
		rectTransform.Position = { 790.0f, 330.0f };
		rectTransform.Size = { 300.0f, 70.0f };
		Entity_AddComponent(&selection, ComponentType::ComponentType_RectTransform, &rectTransform);

		SpriteRendererComponent spriteRenderer = {};
		spriteRenderer.Color = { 0.4f, 0.4f, 0.4f, 0.5f };
		Entity_AddComponent(&selection, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

		Scene_AddEntity(scene, selection);
	}

	{
		Entity light = {};
		light.Tag.Name = "Light_Red";

		RectTransformComponent rectTransform = {};
		rectTransform.Position = { 270.0f, 200.0f };
		rectTransform.Size = { 150.0f, 250.0f };
		Entity_AddComponent(&light, ComponentType::ComponentType_RectTransform, &rectTransform);

		SpriteRendererComponent spriteRenderer = {};
		spriteRenderer.Texture = RefPtr_AddRef(s_RedLight);
		Entity_AddComponent(&light, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

		ScriptComponent scriptComponent = {};
		scriptComponent.OnCreate = LightController_OnCreate;
		scriptComponent.OnUpdate = LightController_OnUpdate;
		scriptComponent.OnDestroy = LightController_OnDestroy;
		scriptComponent.OnCollision = LightController_OnCollision;
		scriptComponent.OnRaycastHit = LightController_OnRaycastHit;
		scriptComponent.OnEnable = LightController_OnEnable;
		scriptComponent.OnDisable = LightController_OnDisable;

		LightData* data = (LightData*)malloc(sizeof(LightData));
		data->Type = LightType::RED;
		scriptComponent.RuntimeData = data;

		Entity_AddComponent(&light, ComponentType::ComponentType_Script, &scriptComponent);

		Scene_AddEntity(scene, light);
	}

	{
		Entity light = {};
		light.Tag.Name = "Light_Green";

		RectTransformComponent rectTransform = {};
		rectTransform.Position = { 1450.0f, 200.0f };
		rectTransform.Size = { 150.0f, 250.0f };
		Entity_AddComponent(&light, ComponentType::ComponentType_RectTransform, &rectTransform);

		SpriteRendererComponent spriteRenderer = {};
		spriteRenderer.Texture = RefPtr_AddRef(s_GreenLight);
		Entity_AddComponent(&light, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

		ScriptComponent scriptComponent = {};
		scriptComponent.OnCreate = LightController_OnCreate;
		scriptComponent.OnUpdate = LightController_OnUpdate;
		scriptComponent.OnDestroy = LightController_OnDestroy;
		scriptComponent.OnCollision = LightController_OnCollision;
		scriptComponent.OnRaycastHit = LightController_OnRaycastHit;
		scriptComponent.OnEnable = LightController_OnEnable;
		scriptComponent.OnDisable = LightController_OnDisable;

		LightData* data = (LightData*)malloc(sizeof(LightData));
		data->Type = LightType::GREEN;
		scriptComponent.RuntimeData = data;

		Entity_AddComponent(&light, ComponentType::ComponentType_Script, &scriptComponent);

		Scene_AddEntity(scene, light);
	}
}

void ScriptGlue_CreatePlayScene(Scene& scene)
{
	#pragma region Textures
	Texture2D texture;
	Texture2D_Create(&texture, "assets/textures/2.png");
	RefPtr* wallTex2 = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/3.png");
	RefPtr* wallTex3 = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/4.png");
	RefPtr* wallTex4 = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/5.png");
	RefPtr* wallTex5 = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/floor.png");
	RefPtr* floorTex = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/roof.png");
	RefPtr* roofTex = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/weapon/shotgun/spritesheet.png");
	RefPtr* shotgunSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/sightIcon.png");
	RefPtr* sightIconSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/caco_demon/walk/spritesheet.png");
	RefPtr* cacoDemonWalkSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/caco_demon/attack/spritesheet.png");
	RefPtr* cacoDemonAttackSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/caco_demon/death/spritesheet.png");
	RefPtr* cacoDemonDeathSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/caco_demon/pain/spritesheet.png");
	RefPtr* cacoDemonPainSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/cyber_demon/walk/spritesheet.png");
	RefPtr* cyberDemonWalkSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/cyber_demon/attack/spritesheet.png");
	RefPtr* cyberDemonAttackSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/cyber_demon/death/spritesheet.png");
	RefPtr* cyberDemonDeathSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/cyber_demon/pain/spritesheet.png");
	RefPtr* cyberDemonPainSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/soldier/walk/spritesheet.png");
	RefPtr* soldierWalkSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/soldier/attack/spritesheet.png");
	RefPtr* soldierAttackSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/soldier/death/spritesheet.png");
	RefPtr* soldierDeathSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/npc/soldier/pain/0.png");
	RefPtr* soldierPainSprite = RefPtr_Create(sizeof(Texture2D), &texture);

	Texture2D_Create(&texture, "assets/textures/medkit.png");
	RefPtr* medkitIcon = RefPtr_Create(sizeof(Texture2D), &texture);
	#pragma endregion

	#pragma region Camera
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
		Entity_AddComponent(&camera, ComponentType::ComponentType_Camera, &cameraComponent);

		Scene_AddEntity(scene, camera);
	}
	#endif
	#pragma endregion

	#pragma region Player
	{
		Entity player = {};
		player.Tag.Name = "Player";
		player.Transform.Translation = { 45.0f, -15.0f, 1.0f };
		player.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
		player.Transform.Scale = { 1.0f, 1.0f, 1.0f };

		Rigidbody2DComponent rigidbody2D = {};
		rigidbody2D.Type = Rigidbody2D::BodyType::Kinematic;
		rigidbody2D.GravityScale = 0.0f;
		Entity_AddComponent(&player, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

		BoxCollider2DComponent boxCollider2D = {};
		boxCollider2D.Restitution = 0.0f;
		boxCollider2D.Offset = { 0,0 };
		boxCollider2D.Size = { 3.0f,1.0f };
		Entity_AddComponent(&player, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

		AudioComponent audioComponent = {};
		audioComponent.Type = AudioComponentType::AudioComponentType_Listener;
		Entity_AddComponent(&player, ComponentType::ComponentType_Audio, &audioComponent);

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
	#pragma endregion

	#pragma region Pickable Items
	{
		{
			Entity enemy = {};
			enemy.Tag.Name = "Item_1";
			enemy.Transform.Translation = { 15.0f,20.0f, 3.5f };
			enemy.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
			enemy.Transform.Scale = { 2.5f, 2.5f, 1.0f };

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Texture = RefPtr_AddRef(medkitIcon);
			Entity_AddComponent(&enemy, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Rigidbody2DComponent rigidbody2D = {};
			rigidbody2D.Type = Rigidbody2D::BodyType::Static;
			Entity_AddComponent(&enemy, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

			BoxCollider2DComponent boxCollider2D = {};
			boxCollider2D.Restitution = 0.0f;
			boxCollider2D.Offset = { 0,0 };
			boxCollider2D.Size = { 0.6f,0.2f };
			boxCollider2D.IsTrigger = true;
			Entity_AddComponent(&enemy, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

			ScriptComponent scriptComponent = {};
			scriptComponent.OnCreate = ItemTrigger_OnCreate;
			scriptComponent.OnUpdate = ItemTrigger_OnUpdate;
			scriptComponent.OnDestroy = ItemTrigger_OnDestroy;
			scriptComponent.OnCollision = ItemTrigger_OnCollision;
			scriptComponent.OnRaycastHit = ItemTrigger_OnRaycastHit;
			scriptComponent.OnEnable = ItemTrigger_OnEnable;
			scriptComponent.OnDisable = ItemTrigger_OnDisable;

			ItemTriggerData* itemData = (ItemTriggerData*)malloc(sizeof(EnemyData));
			*itemData = {};
			itemData->Type = ItemTriggerType::MEDKIT;

			scriptComponent.RuntimeData = itemData;

			Entity_AddComponent(&enemy, ComponentType::ComponentType_Script, &scriptComponent);

			Scene_AddEntity(scene, enemy);
		}
	}
	#pragma endregion

	#pragma region Enemy	
	{
		{
			Entity enemy = {};
			enemy.Tag.Name = "Enemy_Caco_Demon_1";
			enemy.Transform.Translation = { 15.0f,20.0f, 1.5f };
			enemy.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
			enemy.Transform.Scale = { 5.0f, 5.0f, 1.0f };

			SpriteRendererComponent spriteRenderer = {};
			Entity_AddComponent(&enemy, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Rigidbody2DComponent rigidbody2D = {};
			rigidbody2D.Type = Rigidbody2D::BodyType::Kinematic;
			Entity_AddComponent(&enemy, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

			BoxCollider2DComponent boxCollider2D = {};
			boxCollider2D.Restitution = 0.0f;
			boxCollider2D.Offset = { 0,0 };
			boxCollider2D.Size = { 0.6f,0.2f };
			Entity_AddComponent(&enemy, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

			AudioComponent audioComponent = {};
			audioComponent.Type = AudioComponentType::AudioComponentType_Emitter;
			Entity_AddComponent(&enemy, ComponentType::ComponentType_Audio, &audioComponent);

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
			enemyData->WalkSpriteSheet = RefPtr_AddRef(cacoDemonWalkSprite);
			enemyData->AttackSpriteSheet = RefPtr_AddRef(cacoDemonAttackSprite);
			enemyData->DeathSpriteSheet = RefPtr_AddRef(cacoDemonDeathSprite);
			enemyData->PainSpriteSheet = RefPtr_AddRef(cacoDemonPainSprite);

			scriptComponent.RuntimeData = enemyData;

			Entity_AddComponent(&enemy, ComponentType::ComponentType_Script, &scriptComponent);

			Scene_AddEntity(scene, enemy);
		}
		{
			Entity enemy = {};
			enemy.Tag.Name = "Enemy_Cyber_Demon_1";
			enemy.Transform.Translation = { 15.0f,20.0f, 2.0f };
			enemy.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
			enemy.Transform.Scale = { 5.0f, 5.0f, 1.0f };

			SpriteRendererComponent spriteRenderer = {};
			Entity_AddComponent(&enemy, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Rigidbody2DComponent rigidbody2D = {};
			rigidbody2D.Type = Rigidbody2D::BodyType::Kinematic;
			Entity_AddComponent(&enemy, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

			BoxCollider2DComponent boxCollider2D = {};
			boxCollider2D.Restitution = 0.0f;
			boxCollider2D.Offset = { 0,0 };
			boxCollider2D.Size = { 0.6f,0.2f };
			Entity_AddComponent(&enemy, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

			AudioComponent audioComponent = {};
			audioComponent.Type = AudioComponentType::AudioComponentType_Emitter;
			Entity_AddComponent(&enemy, ComponentType::ComponentType_Audio, &audioComponent);

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
			enemyData->Type = EnemyType::CYBER_DEMON;
			enemyData->WalkSpriteSheet = RefPtr_AddRef(cyberDemonWalkSprite);
			enemyData->AttackSpriteSheet = RefPtr_AddRef(cyberDemonAttackSprite);
			enemyData->DeathSpriteSheet = RefPtr_AddRef(cyberDemonDeathSprite);
			enemyData->PainSpriteSheet = RefPtr_AddRef(cyberDemonPainSprite);

			scriptComponent.RuntimeData = enemyData;

			Entity_AddComponent(&enemy, ComponentType::ComponentType_Script, &scriptComponent);

			Scene_AddEntity(scene, enemy);
		}
		{
			Entity enemy = {};
			enemy.Tag.Name = "Enemy_Soldier_1";
			enemy.Transform.Translation = { 15.0f,20.0f, 2.0f };
			enemy.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f), 0, 0 };
			enemy.Transform.Scale = { 5.0f, 5.0f, 1.0f };

			SpriteRendererComponent spriteRenderer = {};
			Entity_AddComponent(&enemy, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Rigidbody2DComponent rigidbody2D = {};
			rigidbody2D.Type = Rigidbody2D::BodyType::Kinematic;
			Entity_AddComponent(&enemy, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

			BoxCollider2DComponent boxCollider2D = {};
			boxCollider2D.Restitution = 0.0f;
			boxCollider2D.Offset = { 0,0 };
			boxCollider2D.Size = { 0.6f,0.2f };
			Entity_AddComponent(&enemy, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

			AudioComponent audioComponent = {};
			audioComponent.Type = AudioComponentType::AudioComponentType_Emitter;
			Entity_AddComponent(&enemy, ComponentType::ComponentType_Audio, &audioComponent);

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
			enemyData->Type = EnemyType::SOLDIER;
			enemyData->WalkSpriteSheet = RefPtr_AddRef(soldierWalkSprite);
			enemyData->AttackSpriteSheet = RefPtr_AddRef(soldierAttackSprite);
			enemyData->DeathSpriteSheet = RefPtr_AddRef(soldierDeathSprite);
			enemyData->PainSpriteSheet = RefPtr_AddRef(soldierPainSprite);

			scriptComponent.RuntimeData = enemyData;

			Entity_AddComponent(&enemy, ComponentType::ComponentType_Script, &scriptComponent);

			Scene_AddEntity(scene, enemy);
		}
	}
	#pragma endregion

	#pragma region UI
	{
		//Background and UI manager
		{
			Entity background = {};
			background.Tag.Name = "Background";

			RectTransformComponent rectTransform = {};
			rectTransform.Position = { 0.0f, 0.0f };
			rectTransform.Size = { 1920.0f, 1080.0f };
			Entity_AddComponent(&background, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Color = { 1.0f, 0.0f, 0.0f, 0.0f };
			Entity_AddComponent(&background, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			ScriptComponent scriptComponent = {};
			scriptComponent.OnCreate = UIController_OnCreate;
			scriptComponent.OnUpdate = UIController_OnUpdate;
			scriptComponent.OnDestroy = UIController_OnDestroy;
			scriptComponent.OnCollision = UIController_OnCollision;
			scriptComponent.OnRaycastHit = UIController_OnRaycastHit;
			scriptComponent.OnEnable = UIController_OnEnable;
			scriptComponent.OnDisable = UIController_OnDisable;
			Entity_AddComponent(&background, ComponentType::ComponentType_Script, &scriptComponent);

			Scene_AddEntity(scene, background);
		}
		//Selection
		{
			Entity selection = {};
			selection.Tag.Name = "Selection";
			selection.Enabled = false;

			RectTransformComponent rectTransform = {};
			rectTransform.Position = { 790.0f, 430.0f };
			rectTransform.Size = { 400.0f, 70.0f };
			Entity_AddComponent(&selection, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Color = { 0.4f, 0.4f, 0.4f, 0.5f };
			Entity_AddComponent(&selection, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, selection);
		}
		{
			Entity weapon = {};
			weapon.Tag.Name = "Weapon";
			RectTransformComponent rectTransform;
			rectTransform.Position = { 695.0f, 0.0f };
			rectTransform.Size = { 500.0f, 500.0f };
			Entity_AddComponent(&weapon, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Texture = RefPtr_AddRef(shotgunSprite);
			Entity_AddComponent(&weapon, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, weapon);
		}
		{
			Entity sightIcon = {};
			sightIcon.Tag.Name = "SightIcon";
			RectTransformComponent rectTransform = {};
			rectTransform.Position = { 935.0f, 515.0f };
			rectTransform.Size = { 50.0f, 50.0f };
			Entity_AddComponent(&sightIcon, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Texture = RefPtr_AddRef(sightIconSprite);
			Entity_AddComponent(&sightIcon, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, sightIcon);
		}
		{
			Entity hpBarBackground = {};
			hpBarBackground.Tag.Name = "HPBarBackground";
			RectTransformComponent rectTransform = {};
			rectTransform.Position = { 100.0f, 990.0f };
			rectTransform.Size = { 500.0f, 25.0f };
			Entity_AddComponent(&hpBarBackground, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Color = { 64.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f,0.8f };
			Entity_AddComponent(&hpBarBackground, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, hpBarBackground);
		}
		{
			Entity hpBarBack = {};
			hpBarBack.Tag.Name = "HPBarBack";
			RectTransformComponent rectTransform = {};
			rectTransform.Position = { 100.0f, 990.0f };
			rectTransform.Size = { 500.0f, 25.0f };
			Entity_AddComponent(&hpBarBack, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Color = { 235.0f / 255.0f, 94.0f / 255.0f, 94.0f / 255.0f,0.8f };
			Entity_AddComponent(&hpBarBack, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, hpBarBack);
		}
		{
			Entity hpBarFront = {};
			hpBarFront.Tag.Name = "HPBarFront";
			RectTransformComponent rectTransform = {};
			rectTransform.Position = { 100.0f, 990.0f };
			rectTransform.Size = { 500.0f, 25.0f };
			Entity_AddComponent(&hpBarFront, ComponentType::ComponentType_RectTransform, &rectTransform);

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Color = { 191.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f,0.8f };
			Entity_AddComponent(&hpBarFront, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, hpBarFront);
		}
	}
	#pragma endregion

	#pragma region Map
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
			Entity_AddComponent(&fieldManager, ComponentType::ComponentType_Script, &scriptComponent);

			Scene_AddEntity(scene, fieldManager);
		}
		{
			Entity wall = {};
			wall.Tag.Name = "BackWall";
			wall.Transform.Translation = { 15.0f,-30.0f,0 };
			wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
			wall.Transform.Scale = { 10.0f,10.0f,1.0f };

			Rigidbody2DComponent rigidbody2D = {};
			rigidbody2D.Type = Rigidbody2D::BodyType::Static;
			Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

			BoxCollider2DComponent boxCollider2D = {};
			boxCollider2D.Restitution = 0.0f;
			boxCollider2D.Size = { 3.5f,0.1f };
			Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

			Scene_AddEntity(scene, wall);
		}
		{
			Entity quad = {};
			quad.Tag.Name = "Floor";
			quad.Transform.Translation = { 15.0f,30.0f,5.0f };
			quad.Transform.Scale = { 150.0f,150.0f,1.0f };

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Texture = RefPtr_AddRef(floorTex);
			spriteRenderer.TilingFactor = 5.0f;
			Entity_AddComponent(&quad, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, quad);
		}
		{
			Entity quad = {};
			quad.Tag.Name = "Roof";
			quad.Transform.Translation = { 15.0f,30.0f,-5.0f };
			quad.Transform.Rotation = { 0,DirectX::XMConvertToRadians(180.0f),0 };
			quad.Transform.Scale = { 150.0f,150.0f,1.0f };

			SpriteRendererComponent spriteRenderer = {};
			spriteRenderer.Texture = RefPtr_AddRef(roofTex);
			spriteRenderer.TilingFactor = 5.0f;
			Entity_AddComponent(&quad, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

			Scene_AddEntity(scene, quad);
		}

		#pragma region MAP_ELEMENT_1
		{
			{
				Entity light = {};
				light.Tag.Name = "MAP_Element_1_Light_1";
				light.Transform.Translation = { -15.0f, 28.0f, 2.7f };
				light.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				light.Transform.Scale = { 1.0f,5.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&light, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.0f,0.1f };
				Entity_AddComponent(&light, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(s_RedLight);
				Entity_AddComponent(&light, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				ScriptComponent scriptComponent = {};
				scriptComponent.OnCreate = LightController_OnCreate;
				scriptComponent.OnUpdate = LightController_OnUpdate;
				scriptComponent.OnDestroy = LightController_OnDestroy;
				scriptComponent.OnCollision = LightController_OnCollision;
				scriptComponent.OnRaycastHit = LightController_OnRaycastHit;
				scriptComponent.OnEnable = LightController_OnEnable;
				scriptComponent.OnDisable = LightController_OnDisable;

				LightData* data = (LightData*)malloc(sizeof(LightData));
				data->Type = LightType::RED;
				scriptComponent.RuntimeData = data;

				Entity_AddComponent(&light, ComponentType::ComponentType_Script, &scriptComponent);

				Scene_AddEntity(scene, light);
			}
			{
				Entity light = {};
				light.Tag.Name = "MAP_Element_1_Light_2";
				light.Transform.Translation = { 45.0f, 28.0f, 2.7f };
				light.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				light.Transform.Scale = { 1.0f,5.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&light, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.0f,0.1f };
				Entity_AddComponent(&light, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(s_GreenLight);
				Entity_AddComponent(&light, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				ScriptComponent scriptComponent = {};
				scriptComponent.OnCreate = LightController_OnCreate;
				scriptComponent.OnUpdate = LightController_OnUpdate;
				scriptComponent.OnDestroy = LightController_OnDestroy;
				scriptComponent.OnCollision = LightController_OnCollision;
				scriptComponent.OnRaycastHit = LightController_OnRaycastHit;
				scriptComponent.OnEnable = LightController_OnEnable;
				scriptComponent.OnDisable = LightController_OnDisable;

				LightData* data = (LightData*)malloc(sizeof(LightData));
				data->Type = LightType::GREEN;
				scriptComponent.RuntimeData = data;

				Entity_AddComponent(&light, ComponentType::ComponentType_Script, &scriptComponent);

				Scene_AddEntity(scene, light);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_1";
				wall.Transform.Translation = { 15.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex5);
				spriteRenderer.Color = { 1.0f,1.0f,1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_2";
				wall.Transform.Translation = { 5.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_3";
				wall.Transform.Translation = { -5.0f,31.5f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Offset = { 0.0f,2.0f };
				boxCollider2D.Size = { 0.5f,0.1f };
				boxCollider2D.IsTrigger = true;
				Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				ScriptComponent scriptComponent = {};
				scriptComponent.OnCreate = FieldTrigger_OnCreate;
				scriptComponent.OnUpdate = FieldTrigger_OnUpdate;
				scriptComponent.OnDestroy = FieldTrigger_OnDestroy;
				scriptComponent.OnCollision = FieldTrigger_OnCollision;
				scriptComponent.OnRaycastHit = FieldTrigger_OnRaycastHit;
				scriptComponent.OnEnable = FieldController_OnEnable;
				scriptComponent.OnDisable = FieldController_OnDisable;

				FieldTriggerData* data = (FieldTriggerData*)malloc(sizeof(FieldTriggerData));
				data->Type = FieldTriggerType::GEN_MAP;
				scriptComponent.RuntimeData = data;

				Entity_AddComponent(&wall, ComponentType::ComponentType_Script, &scriptComponent);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_4";
				wall.Transform.Translation = { -15.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 0.5f,0.1f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_5";
				wall.Transform.Translation = { 25.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 2.5f,0.1f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_6";
				wall.Transform.Translation = { 35.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_7";
				wall.Transform.Translation = { 45.0f,30.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_8";
				wall.Transform.Translation = { -20.0f,0.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 3.0f,0.1f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_9";
				wall.Transform.Translation = { -20.0f,25.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_10";
				wall.Transform.Translation = { -20.0f,15.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_11";
				wall.Transform.Translation = { -20.0f,5.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_12";
				wall.Transform.Translation = { -20.0f,-5.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_13";
				wall.Transform.Translation = { -20.0f,-15.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_14";
				wall.Transform.Translation = { -20.0f,-25.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_15";
				wall.Transform.Translation = { 50.0f,0.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 3.0f,0.1f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_16";
				wall.Transform.Translation = { 50.0f,25.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_17";
				wall.Transform.Translation = { 50.0f,15.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_18";
				wall.Transform.Translation = { 50.0f,5.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_19";
				wall.Transform.Translation = { 50.0f,-5.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_20";
				wall.Transform.Translation = { 50.0f,-15.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_1_Wall_21";
				wall.Transform.Translation = { 50.0f,-25.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex2);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
		}
		#pragma endregion

		#pragma region MAP_ELEMENT_2
		{
			{
				Entity light = {};
				light.Tag.Name = "MAP_Element_2_Light_1";
				light.Transform.Translation = { -15.0f, 88.0f, 2.7f };
				light.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				light.Transform.Scale = { 1.0f,5.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&light, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.0f,0.1f };
				Entity_AddComponent(&light, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(s_GreenLight);
				Entity_AddComponent(&light, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				ScriptComponent scriptComponent = {};
				scriptComponent.OnCreate = LightController_OnCreate;
				scriptComponent.OnUpdate = LightController_OnUpdate;
				scriptComponent.OnDestroy = LightController_OnDestroy;
				scriptComponent.OnCollision = LightController_OnCollision;
				scriptComponent.OnRaycastHit = LightController_OnRaycastHit;
				scriptComponent.OnEnable = LightController_OnEnable;
				scriptComponent.OnDisable = LightController_OnDisable;

				LightData* data = (LightData*)malloc(sizeof(LightData));
				data->Type = LightType::GREEN;
				scriptComponent.RuntimeData = data;

				Entity_AddComponent(&light, ComponentType::ComponentType_Script, &scriptComponent);

				Scene_AddEntity(scene, light);
			}
			{
				Entity light = {};
				light.Tag.Name = "MAP_Element_2_Light_2";
				light.Transform.Translation = { 45.0f, 88.0f, 2.7f };
				light.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				light.Transform.Scale = { 1.0f,5.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&light, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 1.0f,0.1f };
				Entity_AddComponent(&light, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(s_RedLight);
				Entity_AddComponent(&light, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				ScriptComponent scriptComponent = {};
				scriptComponent.OnCreate = LightController_OnCreate;
				scriptComponent.OnUpdate = LightController_OnUpdate;
				scriptComponent.OnDestroy = LightController_OnDestroy;
				scriptComponent.OnCollision = LightController_OnCollision;
				scriptComponent.OnRaycastHit = LightController_OnRaycastHit;
				scriptComponent.OnEnable = LightController_OnEnable;
				scriptComponent.OnDisable = LightController_OnDisable;

				LightData* data = (LightData*)malloc(sizeof(LightData));
				data->Type = LightType::RED;
				scriptComponent.RuntimeData = data;

				Entity_AddComponent(&light, ComponentType::ComponentType_Script, &scriptComponent);

				Scene_AddEntity(scene, light);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_1";
				wall.Transform.Translation = { 15.0f,90.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex4);
				spriteRenderer.Color = { 1.0f,1.0f,1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_2";
				wall.Transform.Translation = { 5.0f,90.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 2.5f,0.1f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_3";
				wall.Transform.Translation = { -5.0f,90.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_4";
				wall.Transform.Translation = { -15.0f,90.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_5";
				wall.Transform.Translation = { 25.0f,90.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_6";
				wall.Transform.Translation = { 35.0f,91.5f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Offset = { 0.0f,2.0f };
				boxCollider2D.Size = { 0.5f,0.1f };
				boxCollider2D.IsTrigger = true;
				Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				ScriptComponent scriptComponent = {};
				scriptComponent.OnCreate = FieldTrigger_OnCreate;
				scriptComponent.OnUpdate = FieldTrigger_OnUpdate;
				scriptComponent.OnDestroy = FieldTrigger_OnDestroy;
				scriptComponent.OnCollision = FieldTrigger_OnCollision;
				scriptComponent.OnRaycastHit = FieldTrigger_OnRaycastHit;
				scriptComponent.OnEnable = FieldController_OnEnable;
				scriptComponent.OnDisable = FieldController_OnDisable;

				FieldTriggerData* data = (FieldTriggerData*)malloc(sizeof(FieldTriggerData));
				data->Type = FieldTriggerType::GEN_MAP;
				scriptComponent.RuntimeData = data;

				Entity_AddComponent(&wall, ComponentType::ComponentType_Script, &scriptComponent);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_7";
				wall.Transform.Translation = { 45.0f,90.0f,0 };
				wall.Transform.Rotation = { -DirectX::XMConvertToRadians(90.0f),0,0 };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 0.5f,0.1f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_8";
				wall.Transform.Translation = { -20.0f,60.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 3.0f,0.1f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_9";
				wall.Transform.Translation = { -20.0f,85.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_10";
				wall.Transform.Translation = { -20.0f,75.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_11";
				wall.Transform.Translation = { -20.0f,65.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_12";
				wall.Transform.Translation = { -20.0f,55.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_13";
				wall.Transform.Translation = { -20.0f,45.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_14";
				wall.Transform.Translation = { -20.0f,35.0f,0 };
				wall.Transform.Rotation = { 0,-DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 0.0f,0.0f };
				spriteRenderer.UVEnd = { 1.0f,1.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_15";
				wall.Transform.Translation = { 50.0f,60.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				Rigidbody2DComponent rigidbody2D = {};
				rigidbody2D.Type = Rigidbody2D::BodyType::Static;
				Entity_AddComponent(&wall, ComponentType::ComponentType_Rigidbody2D, &rigidbody2D);

				BoxCollider2DComponent boxCollider2D = {};
				boxCollider2D.Restitution = 0.0f;
				boxCollider2D.Size = { 3.0f,0.1f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_BoxCollider2D, &boxCollider2D);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_16";
				wall.Transform.Translation = { 50.0f,85.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_17";
				wall.Transform.Translation = { 50.0f,75.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_18";
				wall.Transform.Translation = { 50.0f,65.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_19";
				wall.Transform.Translation = { 50.0f,55.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_20";
				wall.Transform.Translation = { 50.0f,45.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
			{
				Entity wall = {};
				wall = {};
				wall.Tag.Name = "MAP_Element_2_Wall_21";
				wall.Transform.Translation = { 50.0f,35.0f,0 };
				wall.Transform.Rotation = { 0,DirectX::XMConvertToRadians(90.0f),DirectX::XMConvertToRadians(90.0f) };
				wall.Transform.Scale = { 10.0f,10.0f,1.0f };

				SpriteRendererComponent spriteRenderer = {};
				spriteRenderer.Texture = RefPtr_AddRef(wallTex3);
				spriteRenderer.UVStart = { 1.0f,1.0f };
				spriteRenderer.UVEnd = { 0.0f,0.0f };
				Entity_AddComponent(&wall, ComponentType::ComponentType_SpriteRenderer, &spriteRenderer);

				Scene_AddEntity(scene, wall);
			}
		}
		#pragma endregion
	}
	#pragma endregion
}