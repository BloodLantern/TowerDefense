#pragma once

#include "Entity.h"
#include <stdint.h>

class Projectile;
class Enemy;

class Tower : public Entity
{
private:
	float_t m_range;
	float_t m_cooldown;

	Projectile& m_projectileTemplate;
	Enemy* m_target;

	uint8_t m_genericUpgradeLevel;
	uint8_t m_customUpgradeLevel;

public:
	virtual void shoot(const Projectile& projTemplate);
};
