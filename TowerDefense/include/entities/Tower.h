#pragma once

#include "Entity.h"
#include "Projectile.h"
#include "Enemy.h"
#include <stdint.h>

class Tower : public Entity
{
public:
	Tower(Projectile* projectileTemplate);
	Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate);

	virtual void shoot(const Projectile& projTemplate) = 0;

private:
	float_t m_range;
	// Number of attacks per second
	float_t m_attackSpeed;

	Projectile& m_projectileTemplate;
	Enemy* m_target;

	uint8_t m_width;
	uint8_t m_height;

	uint8_t m_genericUpgradeLevel;
	uint8_t m_customUpgradeLevel;

	uint32_t m_damageDealt;
	uint32_t m_killCount;
	uint32_t m_moneyGenerated;
};
