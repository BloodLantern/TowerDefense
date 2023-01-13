#include "projectile.hpp"

Projectile::Projectile(uint32_t damage, uint16_t pierce, float_t lifetime)
	: Entity(), mVelocity(Vector2(0, 0)), mDamage(damage), mPierce(pierce), mLifetime(lifetime)
{
}

void Projectile::OnUpdate()
{

}

void Projectile::OnRender()
{
}
