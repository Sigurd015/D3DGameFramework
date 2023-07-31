#pragma once
#include "Core.h"
#include "Player/PlayerController.h"
#include "Camera/CameraController.h"
#include "UI/UIController.h"

// Hardcoded scene
void ScriptGlue_CreatePlayScene(Scene& scene)
{
	Texture2D* texture = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*texture, "assets/textures/Container_Diffuse.png");

	Texture2D* itemSlotTex = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*itemSlotTex, "assets/textures/ItemSlot.png");

	Texture2D* settingsIcon = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*settingsIcon, "assets/textures/SettingsIcon.png");

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

		ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		*scriptComponent = {};
		scriptComponent->OnCreate = CameraController_OnCreate;
		scriptComponent->OnUpdate = CameraController_OnUpdate;
		scriptComponent->OnDestroy = CameraController_OnDestroy;
		scriptComponent->OnCollision = CameraController_OnCollision;
		Entity_AddComponent(*camera, ComponentType::ComponentType_Script, scriptComponent);

		Scene_AddEntity(scene, *camera);
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

		{
			Entity* vigorBarFront = (Entity*)malloc(sizeof(Entity));
			*vigorBarFront = {};
			vigorBarFront->Tag.Name = "VigorBarFront";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 150.0f, 960.0f };
			rectTransform->Size = { 500.0f, 25.0f };
			Entity_AddComponent(*vigorBarFront, ComponentType::ComponentType_RectTransform, rectTransform);

			SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
			*spriteRenderer = {};
			spriteRenderer->Color = { 20.0f / 255.0f, 210.0f / 255.0f, 20.0f / 255.0f,0.8f };
			Entity_AddComponent(*vigorBarFront, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

			Scene_AddEntity(scene, *vigorBarFront);
		}

		{
			Entity* vigorBarBack = (Entity*)malloc(sizeof(Entity));
			*vigorBarBack = {};
			vigorBarBack->Tag.Name = "VigorBarBack";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 150.0f, 960.0f };
			rectTransform->Size = { 500.0f, 25.0f };
			Entity_AddComponent(*vigorBarBack, ComponentType::ComponentType_RectTransform, rectTransform);

			SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
			*spriteRenderer = {};
			spriteRenderer->Color = { 100.0f / 255.0f, 175.0f / 255.0f, 100.0f / 255.0f,0.8f };
			Entity_AddComponent(*vigorBarBack, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

			Scene_AddEntity(scene, *vigorBarBack);
		}

		{
			Entity* vigorBarBackground = (Entity*)malloc(sizeof(Entity));
			*vigorBarBackground = {};
			vigorBarBackground->Tag.Name = "VigorBarBackground";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 150.0f, 960.0f };
			rectTransform->Size = { 500.0f, 25.0f };
			Entity_AddComponent(*vigorBarBackground, ComponentType::ComponentType_RectTransform, rectTransform);

			SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
			*spriteRenderer = {};
			spriteRenderer->Color = { 64.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f,0.8f };
			Entity_AddComponent(*vigorBarBackground, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

			Scene_AddEntity(scene, *vigorBarBackground);
		}

		{
			Entity* itemSlot = (Entity*)malloc(sizeof(Entity));
			*itemSlot = {};
			itemSlot->Tag.Name = "ItemSlot";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 50.0f, 950.0f };
			rectTransform->Size = { 75.0f, 75.0f };
			Entity_AddComponent(*itemSlot, ComponentType::ComponentType_RectTransform, rectTransform);

			SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
			*spriteRenderer = {};
			spriteRenderer->Texture = itemSlotTex;
			spriteRenderer->Color = { 1.0f,1.0f,1.0f,0.8f };
			Entity_AddComponent(*itemSlot, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

			Scene_AddEntity(scene, *itemSlot);
		}

		{
			Entity* settingIcon = (Entity*)malloc(sizeof(Entity));
			*settingIcon = {};
			settingIcon->Tag.Name = "SettingIcon";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 1700.0f, 960.0f };
			rectTransform->Size = { 75.0f, 75.0f };
			Entity_AddComponent(*settingIcon, ComponentType::ComponentType_RectTransform, rectTransform);

			SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
			*spriteRenderer = {};
			spriteRenderer->Color = { 1.0f,1.0f,1.0f,0.8f };
			spriteRenderer->Texture = settingsIcon;
			Entity_AddComponent(*settingIcon, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

			Scene_AddEntity(scene, *settingIcon);
		}

		{
			Entity* photoIcon = (Entity*)malloc(sizeof(Entity));
			*photoIcon = {};
			photoIcon->Tag.Name = "PhotoIcon";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 1800.0f, 960.0f };
			rectTransform->Size = { 75.0f, 75.0f };
			Entity_AddComponent(*photoIcon, ComponentType::ComponentType_RectTransform, rectTransform);

			SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
			*spriteRenderer = {};
			spriteRenderer->Color = { 1.0f,1.0f,1.0f,0.8f };
			spriteRenderer->Texture = settingsIcon;
			Entity_AddComponent(*photoIcon, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

			Scene_AddEntity(scene, *photoIcon);
		}

		{
			Entity* settingBackground = (Entity*)malloc(sizeof(Entity));
			*settingBackground = {};
			settingBackground->Tag.Name = "SettingBackground";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 1680.0f, 950.0f };
			rectTransform->Size = { 210.0f, 100.0f };
			Entity_AddComponent(*settingBackground, ComponentType::ComponentType_RectTransform, rectTransform);

			SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
			*spriteRenderer = {};
			spriteRenderer->Color = { 0.5f,0.5f,0.5f,0.6f };
			Entity_AddComponent(*settingBackground, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

			Scene_AddEntity(scene, *settingBackground);
		}

		{
			Entity* testText = (Entity*)malloc(sizeof(Entity));
			*testText = {};
			testText->Tag.Name = "TestText1";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 50.0f, 200.0f };
			Entity_AddComponent(*testText, ComponentType::ComponentType_RectTransform, rectTransform);

			TextComponent* textComponent = (TextComponent*)malloc(sizeof(TextComponent));
			*textComponent = {};
			textComponent->Color = { 1.0f,1.0f,1.0f,1.0f };
			textComponent->TextString = L"Hellow World 123456";
			textComponent->FontSize = 150.0f;
			Entity_AddComponent(*testText, ComponentType::ComponentType_Text, textComponent);

			Scene_AddEntity(scene, *testText);
		}

		{
			Entity* testText = (Entity*)malloc(sizeof(Entity));
			*testText = {};
			testText->Tag.Name = "TestText2";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 50.0f, 400.0f };
			Entity_AddComponent(*testText, ComponentType::ComponentType_RectTransform, rectTransform);

			TextComponent* textComponent = (TextComponent*)malloc(sizeof(TextComponent));
			*textComponent = {};
			textComponent->Color = { 0.0f,0.0f,0.0f,1.0f };
			textComponent->TextString = L"’Š“’f……X—¬,ŽØŽðÁDDXD";
			textComponent->FontSize = 100.0f;
			Entity_AddComponent(*testText, ComponentType::ComponentType_Text, textComponent);

			Scene_AddEntity(scene, *testText);
		}

		{
			Entity* testText = (Entity*)malloc(sizeof(Entity));
			*testText = {};
			testText->Tag.Name = "TestText3";
			RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
			*rectTransform = {};
			rectTransform->Position = { 50.0f, 600.0f };
			Entity_AddComponent(*testText, ComponentType::ComponentType_RectTransform, rectTransform);

			TextComponent* textComponent = (TextComponent*)malloc(sizeof(TextComponent));
			*textComponent = {};
			textComponent->Color = { 1.0f,0.0f,0.0f,1.0f };
			textComponent->TextString = L"‚ ‚¢‚¤‚¦‚¨";
			textComponent->FontSize = 50.0f;
			Entity_AddComponent(*testText, ComponentType::ComponentType_Text, textComponent);

			Scene_AddEntity(scene, *testText);
		}
	}

	{
		Entity* quad = (Entity*)malloc(sizeof(Entity));
		*quad = {};
		quad->Tag.Name = "Quad-1";
		quad->Transform.Translation.x = -8.0f;
		quad->Transform.Scale = { 2.0f,2.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = texture;
		Entity_AddComponent(*quad, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		Entity_AddComponent(*quad, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		BoxCollider2DComponent* boxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		*boxCollider2D = {};
		boxCollider2D->Restitution = 0.5f;
		Entity_AddComponent(*quad, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		Scene_AddEntity(scene, *quad);
	}

	{
		Entity* quad = (Entity*)malloc(sizeof(Entity));
		*quad = {};
		quad->Tag.Name = "Quad-2";
		quad->Transform.Translation.x = -10.5f;
		quad->Transform.Scale = { 1.0f,1.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = texture;
		Entity_AddComponent(*quad, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		rigidbody2D->Type = Rigidbody2D::BodyType::Dynamic;
		Entity_AddComponent(*quad, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		BoxCollider2DComponent* boxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		*boxCollider2D = {};
		boxCollider2D->IsTrigger = true;
		Entity_AddComponent(*quad, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		Scene_AddEntity(scene, *quad);
	}

	{
		Entity* quad = (Entity*)malloc(sizeof(Entity));
		*quad = {};
		quad->Tag.Name = "Ground1";
		quad->Transform.Translation.y = -15.5f;
		quad->Transform.Scale = { 100.0f,1.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		Entity_AddComponent(*quad, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		rigidbody2D->Type = Rigidbody2D::BodyType::Static;
		Entity_AddComponent(*quad, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		BoxCollider2DComponent* boxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		*boxCollider2D = {};
		boxCollider2D->Restitution = 0.5f;
		Entity_AddComponent(*quad, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		Scene_AddEntity(scene, *quad);
	}

	{
		Entity* quad = (Entity*)malloc(sizeof(Entity));
		*quad = {};
		//quad->Tag.Name = "Quad-3";
		quad->Tag.Name = "Player";
		quad->Transform.Translation.x = -15.5f;
		quad->Transform.Translation.y = 10.5f;
		quad->Transform.Scale = { 1.0f,1.0f,1.0f };

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = texture;
		Entity_AddComponent(*quad, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		rigidbody2D->Type = Rigidbody2D::BodyType::Kinematic;
		Entity_AddComponent(*quad, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		BoxCollider2DComponent* boxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		*boxCollider2D = {};
		boxCollider2D->Restitution = 0.0f;
		Entity_AddComponent(*quad, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		*scriptComponent = {};
		scriptComponent->OnCreate = PlayerController_OnCreate;
		scriptComponent->OnUpdate = PlayerController_OnUpdate;
		scriptComponent->OnDestroy = PlayerController_OnDestroy;
		scriptComponent->OnCollision = PlayerController_OnCollision;
		Entity_AddComponent(*quad, ComponentType::ComponentType_Script, scriptComponent);

		Scene_AddEntity(scene, *quad);
	}

	{
		Entity* circle = (Entity*)malloc(sizeof(Entity));
		*circle = {};
		circle->Tag.Name = "Circle-1";
		circle->Transform.Translation.x = -1.0f;
		circle->Transform.Scale = { 2.5f,2.5f,1.0f };

		CircleRendererComponent* circleRenderer = (CircleRendererComponent*)malloc(sizeof(CircleRendererComponent));
		*circleRenderer = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleRenderer, circleRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		rigidbody2D->Type = Rigidbody2D::BodyType::Dynamic;
		Entity_AddComponent(*circle, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		CircleCollider2DComponent* circleCollider2D = (CircleCollider2DComponent*)malloc(sizeof(CircleCollider2DComponent));
		*circleCollider2D = {};
		circleCollider2D->Restitution = 0.5f;
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleCollider2D, circleCollider2D);

		Scene_AddEntity(scene, *circle);
	}

	{
		Entity* circle = (Entity*)malloc(sizeof(Entity));
		*circle = {};
		circle->Tag.Name = "Circle-2";
		circle->Transform.Translation.x = 2.0f;
		circle->Transform.Scale = { 2.5f,2.5f,1.0f };

		CircleRendererComponent* circleRenderer = (CircleRendererComponent*)malloc(sizeof(CircleRendererComponent));
		*circleRenderer = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleRenderer, circleRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		CircleCollider2DComponent* circleCollider2D = (CircleCollider2DComponent*)malloc(sizeof(CircleCollider2DComponent));
		*circleCollider2D = {};
		circleCollider2D->Restitution = 0.5f;
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleCollider2D, circleCollider2D);

		Scene_AddEntity(scene, *circle);
	}

	{
		Entity* circle = (Entity*)malloc(sizeof(Entity));
		*circle = {};
		circle->Tag.Name = "Circle-3";
		//circle->Tag.Name = "Player";
		circle->Transform.Translation.x = 5.0f;
		circle->Transform.Translation.z = -0.1f;
		circle->Transform.Scale = { 1.0f,1.0f,1.0f };

		CircleRendererComponent* circleRenderer = (CircleRendererComponent*)malloc(sizeof(CircleRendererComponent));
		*circleRenderer = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleRenderer, circleRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		rigidbody2D->Type = Rigidbody2D::BodyType::Dynamic;
		Entity_AddComponent(*circle, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		CircleCollider2DComponent* circleCollider2D = (CircleCollider2DComponent*)malloc(sizeof(CircleCollider2DComponent));
		*circleCollider2D = {};
		circleCollider2D->Restitution = 0.5f;
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleCollider2D, circleCollider2D);

		//ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		//*scriptComponent = {};
		//scriptComponent->OnCreate = PlayerController_OnCreate;
		//scriptComponent->OnUpdate = PlayerController_OnUpdate;
		//scriptComponent->OnDestroy = PlayerController_OnDestroy;
		//scriptComponent->OnCollision = PlayerController_OnCollision;
		//Entity_AddComponent(*circle, ComponentType::ComponentType_Script, scriptComponent);

		Scene_AddEntity(scene, *circle);
	}
}

void ScriptGlue_CreateTitleScene(Scene& scene)
{}