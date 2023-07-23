#include "ScriptGlue.h"
#include "Core.h"
#include "Player/PlayerController.h"
#include "Camera/CameraController.h"

void ScriptGlue_Ininialize(Scene& scene)
{
	// Hardcoded scene
	Texture2D* texture = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*texture, "assets/textures/Container_Diffuse.png");

	{
		Entity* camera = (Entity*)malloc(sizeof(Entity));
		*camera = {};
		camera->Tag.Name = "Main-Camera";
		camera->Transform.Translation = { 0.0f, 0.0f, -10.0f };

		CameraSpecification spec;
		spec.ProjectionType = ProjectionType::Orthographic;
		spec.AspectRatio = 16.0f / 9.0f;
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
		Entity_AddComponent(*quad, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		//ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		//*scriptComponent = {};
		//scriptComponent->OnCreate = PlayerController_OnCreate;
		//scriptComponent->OnUpdate = PlayerController_OnUpdate;
		//scriptComponent->OnDestroy = PlayerController_OnDestroy;
		//scriptComponent->OnCollision = PlayerController_OnCollision;
		//Entity_AddComponent(*quad, ComponentType::ComponentType_Script, scriptComponent);

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
		Entity_AddComponent(*quad, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		BoxCollider2DComponent* boxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		*boxCollider2D = {};
		Entity_AddComponent(*quad, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		Scene_AddEntity(scene, *quad);
	}

	{
		Entity* circle = (Entity*)malloc(sizeof(Entity));
		*circle = {};
		circle->Tag.Name = "Circle-1";
		circle->Transform.Translation.x = 0.0f;
		circle->Transform.Scale = { 2.5f,2.5f,1.0f };

		CircleRendererComponent* circleRenderer = (CircleRendererComponent*)malloc(sizeof(CircleRendererComponent));
		*circleRenderer = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleRenderer, circleRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		CircleCollider2DComponent* circleCollider2D = (CircleCollider2DComponent*)malloc(sizeof(CircleCollider2DComponent));
		*circleCollider2D = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleCollider2D, circleCollider2D);

		Scene_AddEntity(scene, *circle);
	}

	{
		Entity* circle = (Entity*)malloc(sizeof(Entity));
		*circle = {};
		circle->Tag.Name = "Player";
		circle->Transform.Translation.x = 3.0f;
		circle->Transform.Translation.z = -0.1f;
		circle->Transform.Scale = { 1.0f,1.0f,1.0f };

		CircleRendererComponent* circleRenderer = (CircleRendererComponent*)malloc(sizeof(CircleRendererComponent));
		*circleRenderer = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleRenderer, circleRenderer);

		Rigidbody2DComponent* rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		*rigidbody2D = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_Rigidbody2D, rigidbody2D);

		CircleCollider2DComponent* circleCollider2D = (CircleCollider2DComponent*)malloc(sizeof(CircleCollider2DComponent));
		*circleCollider2D = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleCollider2D, circleCollider2D);

		ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		*scriptComponent = {};
		scriptComponent->OnCreate = PlayerController_OnCreate;
		scriptComponent->OnUpdate = PlayerController_OnUpdate;
		scriptComponent->OnDestroy = PlayerController_OnDestroy;
		scriptComponent->OnCollision = PlayerController_OnCollision;
		Entity_AddComponent(*circle, ComponentType::ComponentType_Script, scriptComponent);

		Scene_AddEntity(scene, *circle);
	}
}