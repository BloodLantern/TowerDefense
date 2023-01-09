#pragma once

#include "Entity.h"
#include "Projectile.h"
#include "Enemy.h"

#include <stdint.h>

class Tower : public Entity
{
public:
	Tower(const Tower& other);
	Tower(Projectile* projectileTemplate);
	Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate);
	virtual ~Tower() {};

	virtual void shoot(const Projectile& projTemplate);
	void draw() override;

    float_t getRange() const { return m_range; }
	uint8_t getWidth() const { return m_width; }
	uint8_t getHeight() const { return m_height; }

protected:
	void setRange(float range) { m_range = range; }
	void setWidth(uint8_t newWidth) { m_width = newWidth; }
	void setHeight(uint8_t newHeight) { m_height = newHeight; }

private:
	float_t m_range = 3.f;
	// Number of attacks per second
	float_t m_attackSpeed = 1.f;

	Projectile& m_projectileTemplate;
	Enemy* m_target;

	uint8_t m_genericUpgradeLevel = 1;
	uint8_t m_customUpgradeLevel = 1;

	uint8_t m_width = 1;
	uint8_t m_height = 1;

	uint32_t m_damageDealt = 0;
	uint32_t m_killCount = 0;
	uint32_t m_moneyGenerated = 0;
};
