#pragma once

#include "Entity.h"
#include <stdint.h>
#include <Vector2.h>

#define PROJECTILE_DEFAULT_POSITION Point2(-150, -150)

class Projectile : public Entity
{
public:
	Projectile(uint32_t damage, uint16_t pierce, float_t lifetime);

	void draw() override;

private:
	Vector2 m_velocity;
	uint32_t m_damage;
	uint16_t m_pierce;
	float_t m_lifetime;
};
