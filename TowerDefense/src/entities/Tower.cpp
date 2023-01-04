#include "Tower.h"

Tower::Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate)
	: Entity(pixelPosition), m_range(range), m_attackSpeed(attackSpeed), m_projectileTemplate(*projectileTemplate)
{
	m_customUpgradeLevel = 0;
	m_genericUpgradeLevel = 0;

	m_target = nullptr;
}
