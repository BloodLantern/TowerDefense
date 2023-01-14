#pragma once

#include "enemy.hpp"
#include <stdint.h>
#include <Vector2.h>

class Tower;

#define PROJECTILE_DEFAULT_POSITION Point2(NAN, NAN)

class Projectile : public Entity
{
public:
	Projectile(const Projectile& other);
	Projectile(float_t speed, uint32_t damage, uint16_t pierce, float_t lifetime);

	virtual void OnRender() override;
	virtual void OnUpdate() override;

	Vector2 GetVelocity() const { return mVelocity; }
	void SetVelocity(Vector2 velocity) { mVelocity = velocity; }
	void SetTarget(Enemy* target) { mTarget = target; }
	void SetDamage(uint32_t damage) { mDamage = damage; }
	void SetOwner(Tower* owner) { mOwner = owner; }

private:
	Vector2 mVelocity;
	
	float_t mSpeed;
	uint32_t mDamage;
	uint16_t mPierce;
	float_t mLifetime;

	Enemy* mTarget;
	Tower* mOwner;
	std::vector<Enemy*> mHitEnemies;
	
	void HandleEnemyCollision();
};
