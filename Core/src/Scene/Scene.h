#pragma once
#include "Entity.h"

struct Scene
{
	List Entities;
};

Entity* Scene_GetEntityByName(const Scene& scene, const char* name);

void Scene_OnUpdate(const Scene& scene, float timeStep);
void Scene_OnViewportResize(const Scene& scene, uint32_t width, uint32_t height);