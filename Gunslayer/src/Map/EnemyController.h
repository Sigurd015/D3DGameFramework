#pragma once
#include "Core.h"

enum EnemyType
{
	CACO_DEMON, CYBER_DEMON, SOLDIER
};

enum EnemyState
{
	ENEMY_IDLE, ENEMY_WALK, ENEMY_ATTACK, ENEMY_PAIN, ENEMY_DEATH
};

struct EnemyStats
{
	float MaxHP = 0.0f;
	float HP = 0.0f;
};

struct EnemyData
{
	EnemyType Type;
	EnemyState State = ENEMY_IDLE;
	bool CanAttack = true;
	float AttackTimer = 0.0f;
	float AttackCooldown = 0.0f;
	EnemyStats Stats;

	uint32_t Score = 0;

	float AttackDistance = 0.0f;
	float WalkSpeed = 0.0f;
	float AttackRange = 0.0f;

	TransformComponent* PlayerTransform = nullptr;
	TransformComponent* Transform = nullptr;

	Vec2 Forward = { 0.0f, -1.0f };

	Rigidbody2DComponent* Rigidbody = nullptr;
	SpriteRendererComponent* SpriteRenderer = nullptr;

	AudioComponent* Audio = nullptr;
	void* AttackSoundEffect = nullptr;
	void* PainSoundEffect = nullptr;
	void* DeathSoundEffect = nullptr;
	void* WalkSoundEffect = nullptr;

	RefPtr* WalkSpriteSheet = nullptr;
	SpriteAnimator WalkSpriteAnimator;
	SpriteTimer WalkSpriteTimer;
	uint32_t WalkSpriteMaxAnimationFrames;

	RefPtr* AttackSpriteSheet = nullptr;
	SpriteAnimator AttackSpriteAnimator;
	SpriteTimer AttackSpriteTimer;
	uint32_t AttackSpriteMaxAnimationFrames;

	RefPtr* DeathSpriteSheet = nullptr;
	SpriteAnimator DeathSpriteAnimator;
	SpriteTimer DeathSpriteTimer;
	uint32_t DeathSpriteMaxAnimationFrames;

	RefPtr* PainSpriteSheet = nullptr;
	SpriteAnimator PainSpriteAnimator;
	SpriteTimer PainSpriteTimer;
	uint32_t PainSpriteMaxAnimationFrames;

	RefPtr* IdleSpriteSheet = nullptr;
	Vec2 IdleUVStart;
	Vec2 IdleUVEnd;
};

void EnemyController_OnCreate(Entity* entity, void* runtimeData);
void EnemyController_OnUpdate(Entity* entity, float timeStep, void* runtimeData);
void EnemyController_OnDestroy(Entity* entity, void* runtimeData);
void EnemyController_OnCollision(Entity* entity, Entity* other, void* runtimeData);
void EnemyController_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData);
void EnemyController_OnEnable(Entity* entity, void* runtimeData);
void EnemyController_OnDisable(Entity* entity, void* runtimeData);