#include "ScriptGlue.h"
#include "Core.h"
#include "Player/PlayerController.h"

void ScriptGlue_Ininialize(Scene& scene)
{
	// Hardcoded scene
	{
		Entity* camera = (Entity*)malloc(sizeof(Entity));
		*camera = {};
		camera->Tag.Name = "Main Camera";

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

		Scene_AddEntity(scene, *camera);
	}

	{
		Entity* quad = (Entity*)malloc(sizeof(Entity));
		*quad = {};
		quad->Tag.Name = "Player";

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		Entity_AddComponent(*quad, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		*scriptComponent = {};
		scriptComponent->OnCreate = PlayerController_OnCreate;
		scriptComponent->OnUpdate = PlayerController_OnUpdate;
		scriptComponent->OnDestroy = PlayerController_OnDestroy;
		scriptComponent->OnCollision = PlayerController_OnCollision;
		Entity_AddComponent(*quad, ComponentType::ComponentType_Script, scriptComponent);

		BoxCollider2DComponent* boxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		*boxCollider2D = {};
		Entity_AddComponent(*quad, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		Scene_AddEntity(scene, *quad);
	}

	{
		Entity* quad = (Entity*)malloc(sizeof(Entity));
		*quad = {};
		quad->Tag.Name = "Test Quad";
		quad->Transform.Translation.x = -2.5f;

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		Entity_AddComponent(*quad, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);

		BoxCollider2DComponent* boxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		*boxCollider2D = {};
		Entity_AddComponent(*quad, ComponentType::ComponentType_BoxCollider2D, boxCollider2D);

		Scene_AddEntity(scene, *quad);
	}

	{
		Entity* circle = (Entity*)malloc(sizeof(Entity));
		*circle = {};
		circle->Tag.Name = "Test Circle";
		circle->Transform.Translation.x = -4.0f;

		CircleRendererComponent* circleRenderer = (CircleRendererComponent*)malloc(sizeof(CircleRendererComponent));
		*circleRenderer = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleRenderer, circleRenderer);

		CircleCollider2DComponent* circleCollider2D = (CircleCollider2DComponent*)malloc(sizeof(CircleCollider2DComponent));
		*circleCollider2D = {};
		Entity_AddComponent(*circle, ComponentType::ComponentType_CircleCollider2D, circleCollider2D);

		Scene_AddEntity(scene, *circle);
	}
}