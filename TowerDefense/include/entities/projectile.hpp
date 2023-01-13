#pragma once

#include "enemy.hpp"
#include <stdint.h>
#include <Vector2.h>

#define PROJECTILE_DEFAULT_POSITION Point2(NAN, NAN)

class Projectile : public Entity
{
public:
	Projectile(uint32_t damage, uint16_t pierce, float_t lifetime);

	virtual void OnRender() override;
	virtual void OnUpdate() override;

private:
	Vector2 mVelocity;
	uint32_t mDamage;
	uint16_t mPierce;
	float_t mLifetime;
	Enemy* mTarget;
};