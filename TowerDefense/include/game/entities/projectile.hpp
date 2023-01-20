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
	uint16_t GetPierce() const { return mPierce; }
	void SetPierce(uint16_t pierce) { mPierce = pierce; }
	Tower* GetOwner() const { return mOwner; }
	void SetOwner(Tower* owner) { mOwner = owner; }
	void SetSpeed(float_t speed) { mSpeed = speed; }
	float_t GetLifetime() const { return mLifetime; }
	void SetHitboxRadius(float_t radius) { mHitboxRadius = radius; }
	float_t GetHitboxRadius() const { return mHitboxRadius; }
	void SetSlowingEnemies() { mSlowEnemies = true; }

protected:
	std::vector<Enemy*> mHitEnemies;
	
	uint32_t mDamage;
	uint16_t mPierce;
	bool mSlowEnemies;

	float_t mHitboxRadius = 20.f;

	Enemy* mTarget = nullptr;
	Tower* mOwner = nullptr;

	virtual void HandleEnemyCollision();
	Vector2 mVelocity;
	
	float_t mSpeed;
	float_t mLifetime;
};
