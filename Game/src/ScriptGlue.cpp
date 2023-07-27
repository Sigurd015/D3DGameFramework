#include "ScriptGlue.h"
#include "Core.h"
#include "Player/PlayerController.h"
#include "Camera/CameraController.h"

void ScriptGlue_Ininialize(Scene& scene)
{
	// Hardcoded scene
	Texture2D* texture = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*texture, "assets/textures/Container_Diffuse.png");

	Texture2D* itemSlotTex = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*itemSlotTex, "assets/textures/ItemSlot.png");

	Entity* camera = (Entity*)malloc(sizeof(Entity));
	*camera = {};
	camera->Tag.Name = "Main-Camera";
	camera->Transform.Translation = { 0.0f, 0.0f, -10.0f };
	{
		CameraSpecification spec;
		spec.ProjectionType = ProjectionType::Orthographic;
		spec.AspectRatio = 16.0f / 9.0f;
		spec.OrthographicSize = 20.0f;
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

	{
		Entity* hpBar = (Entity*)malloc(sizeof(Entity));
		*hpBar = {};
		hpBar->Tag.Name = "HPBar";

		RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
		*rectTransform = {};
		rectTransform->StandardScreenSize = { 1920.0f,1080.0f };
		rectTransform->Position = { 200.0f, 980.0f };
		rectTransform->Size = { 500.0f, 50.0f };
		Entity_AddComponent(*hpBar, ComponentType::ComponentType_RectTransform, rectTransform);

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Color = { 207.0f, 94.0f, 94.0f,0.8f };
		Entity_AddComponent(*hpBar, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *hpBar);
	}

	{
		Entity* vigorBar = (Entity*)malloc(sizeof(Entity));
		*vigorBar = {};
		vigorBar->Tag.Name = "VigorBar";

		RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
		*rectTransform = {};
		rectTransform->StandardScreenSize = { 1920.0f,1080.0f };
		rectTransform->Position = { 200.0f, 940.0f };
		rectTransform->Size = { 500.0f, 50.0f };
		Entity_AddComponent(*vigorBar, ComponentType::ComponentType_RectTransform, rectTransform);

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		Entity_AddComponent(*vigorBar, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *vigorBar);
	}

	{
		Entity* itemSlot = (Entity*)malloc(sizeof(Entity));
		*itemSlot = {};
		itemSlot->Tag.Name = "ItemSlot";

		RectTransformComponent* rectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
		*rectTransform = {};
		rectTransform->StandardScreenSize = { 1920.0f,1080.0f };
		rectTransform->Position = { 100.0f, 940.0f };
		rectTransform->Size = { 150.0f, 150.0f };
		Entity_AddComponent(*itemSlot, ComponentType::ComponentType_RectTransform, rectTransform);

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		spriteRenderer->Texture = itemSlotTex;
		spriteRenderer->Color = { 1.0f,1.0f,1.0f,0.8f };
		Entity_AddComponent(*itemSlot, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		Scene_AddEntity(scene, *itemSlot);
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
		boxCollider2D->Restitution = 0.5f;
		Entity_AddComponent(*quad, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		Scene_AddEntity(scene, *quad);
	}

	{
		Entity* quad = (Entity*)malloc(sizeof(Entity));
		*quad = {};
		quad->Tag.Name = "Ground";
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