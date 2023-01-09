#pragma once

#include "Entity.h"
#include <stdint.h>
#include <Vector2.h>

#define PROJECTILE_DEFAULT_POSITION Point2(NAN, NAN)

class Projectile : public Entity
{
public:
	Projectile(uint32_t damage, uint16_t pierce, float_t lifetime);

	void Draw() override;

private:
	Vector2 mVelocity;
	uint32_t mDamage;
	uint16_t mPierce;
	float_t mLifetime;
};
