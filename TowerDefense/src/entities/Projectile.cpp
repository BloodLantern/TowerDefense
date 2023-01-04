#include "Projectile.h"

Projectile::Projectile(uint32_t damage, uint16_t pierce, float_t lifetime)
	: Entity(PROJECTILE_DEFAULT_POSITION), m_velocity(Vector2(0, 0)), m_damage(damage), m_pierce(pierce), m_lifetime(lifetime)
{
}

void Projectile::draw()
{
	if (getPixelPosition() == PROJECTILE_DEFAULT_POSITION)
		return;
}
