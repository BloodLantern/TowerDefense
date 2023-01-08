#include "Projectile.h"

Projectile::Projectile(uint32_t damage, uint16_t pierce, float_t lifetime)
	: Entity(), m_velocity(Vector2(0, 0)), m_damage(damage), m_pierce(pierce), m_lifetime(lifetime)
{
}

void Projectile::draw()
{
}
