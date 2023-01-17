#pragma once

#include "enemy.hpp"
#include <stdint.h>
#include <Vector2.h>

class Tower;

#define PROJECTILE_DEFAULT_POSITION Point2(NAN, NAN)
#define PROJECTILE_COLLISION_RADIUS 20.f

class Projectile : public Entity
{
public:
	Projectile(float_t speed, uint32_t damage, uint16_t pierce, float_t lifetime);

	virtual Projectile* Clone() const;

	virtual void OnRender() override;
	virtual void OnUpdate() override;

	Vector2 GetVelocity() const { return mVelocity; }
	void SetVelocity(Vector2 velocity) { mVelocity = velocity; }
	void SetTarget(Enemy* target) { mTarget = target; }
	uint32_t GetDamage() const { return mDamage; }
	void SetDamage(uint32_t damage) { mDamage = damage; }
	Tower* GetOwner() const { return mOwner; }
	void SetOwner(Tower* owner) { mOwner = owner; }

protected:
	std::vector<Enemy*> mHitEnemies;
	
	uint32_t mDamage;
	uint16_t mPierce;

	Enemy* mTarget = nullptr;
	Tower* mOwner = nullptr;

	virtual void HandleEnemyCollision();
	Vector2 mVelocity;
	
	float_t mSpeed;
	float_t mLifetime;
};
