#include "projectile.hpp"
#include "globals.hpp"

Projectile::Projectile(const Projectile &other)
    : Entity(other), mSpeed(other.mSpeed), mDamage(other.mDamage), mPierce(other.mPierce), mLifetime(other.mLifetime)
{
}

Projectile::Projectile(float_t speed, uint32_t damage, uint16_t pierce, float_t lifetime)
    : Entity(), mVelocity(Vector2(0, 0)), mSpeed(speed), mDamage(damage), mPierce(pierce), mLifetime(lifetime)
{
}

void Projectile::OnUpdate()
{
	// If the projectile lifetime is over, destroy it
	if (mLifetime <= 0.f)
	{
		toDelete = true;
		return;
	}

	// Compute velocity
	mVelocity = Vector2(GetPixelPosition(), mTarget->GetPixelPosition()).Normalize() * 60;
	SetPixelPosition(GetPixelPosition() + mVelocity * mSpeed * Globals::gGame->GetPlayingSpeedDeltaTime());

    // Update lifetime
	mLifetime -= Globals::gGame->GetPlayingSpeedDeltaTime();
}

void Projectile::OnRender()
{
	Globals::gDrawList->AddCircleFilled(ImVec2(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y), 10, IM_COL32(0xFF, 0x0, 0x0, 0xFF));
}
