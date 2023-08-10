#pragma once
#include "Core.h"

enum EnemyType
{
	CACO_DEMON, CYBER_DEMON, SOLDIER
};

struct EnemyData
{
	EnemyType Type;

	Rigidbody2DComponent* Rigidbody = nullptr;
	SpriteRendererComponent* SpriteRenderer = nullptr;

	Texture2D* WalkSpriteSheet = nullptr;
	SpriteAnimator WalkSpriteAnimator;
	SpriteTimer WalkSpriteTimer;
	uint32_t WalkSpriteMaxAnimationFrames;

	Texture2D* AttackSpriteSheet = nullptr;
	SpriteAnimator AttackSpriteAnimator;
	SpriteTimer AttackSpriteTimer;
	uint32_t AttackSpriteMaxAnimationFrames;
	bool CanAttack = false;

	Texture2D* DeathSpriteSheet = nullptr;
	SpriteAnimator DeathSpriteAnimator;
	SpriteTimer DeathSpriteTimer;
	uint32_t DeathSpriteMaxAnimationFrames;

	Texture2D* PainSpriteSheet = nullptr;
	SpriteAnimator PainSpriteAnimator;
	SpriteTimer PainSpriteTimer;
	uint32_t PainSpriteMaxAnimationFrames;
	bool IsHited = false;

	Texture2D* IdleSpriteSheet = nullptr;
	Vec2 IdleUVStart;
	Vec2 IdleUVEnd;
};

void EnemyController_OnCreate(Entity& entity, void* runtimeData);
void EnemyController_OnUpdate(Entity& entity, float timeStep, void* runtimeData);
void EnemyController_OnDestroy(Entity& entity, void* runtimeData);
void EnemyController_OnCollision(Entity& entity, Entity& other, void* runtimeData);
void EnemyController_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData);