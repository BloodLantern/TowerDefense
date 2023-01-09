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

	uint8_t getWidth() const { return m_width; }
	uint8_t getHeight() const { return m_height; }

protected:
	void setWidth(uint8_t newWidth) { m_width = newWidth; }
	void setHeight(uint8_t newHeight) { m_height = newHeight; }

private:
	float_t m_range;
	// Number of attacks per second
	float_t m_attackSpeed;

	Projectile& m_projectileTemplate;
	Enemy* m_target;

	uint8_t m_genericUpgradeLevel;
	uint8_t m_customUpgradeLevel;

	uint8_t m_width;
	uint8_t m_height;

	uint32_t m_damageDealt;
	uint32_t m_killCount;
	uint32_t m_moneyGenerated;
};
