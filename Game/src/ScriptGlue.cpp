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
		Entity_AddComponent(camera, ComponentType::ComponentType_Camera, cameraComponent);

		Scene_AddEntity(scene, *camera);
	}
	{
		Entity* quad = (Entity*)malloc(sizeof(Entity));
		*quad = {};
		quad->Tag.Name = "Test Quad";

		SpriteRendererComponent* spriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		*spriteRenderer = {};
		Entity_AddComponent(quad, ComponentType::ComponentType_SpriteRenderer, spriteRenderer);;

		ScriptComponent* scriptComponent = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		*scriptComponent = {};
		scriptComponent->OnCreate = PlayerController_OnCreate;
		scriptComponent->OnUpdate = PlayerController_OnUpdate;
		scriptComponent->OnDestroy = PlayerController_OnDestroy;
		scriptComponent->OnCollision = PlayerController_OnCollision;
		Entity_AddComponent(quad, ComponentType::ComponentType_Script, scriptComponent);

		Scene_AddEntity(scene, *quad);
	}
}

void ScriptGlue_Shutdown()
{}