#include "projectile.hpp"
#include "globals.hpp"

#include <algorithm>

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
	if (mTarget && !mTarget->toDelete)
	{
		// Update velocity
		mVelocity = Vector2(GetPixelPosition(), mTarget->GetPixelPosition()).Normalize() * 60;
	}

	// If the projectile hit an enemy, deal its damage and reduce its pierce
	for (std::vector<Enemy*>::iterator it = Globals::gGame->enemies.begin(); it != Globals::gGame->enemies.end(); ++it)
	{
		Enemy* enemy = *it;
		if (std::find(mHitEnemies.begin(), mHitEnemies.end(), enemy) != mHitEnemies.end())
			continue;

		if (Vector2(enemy->GetPixelPosition(), GetPixelPosition()).GetSquaredNorm() < 20.f * 20.f)
		{
			enemy->DealDamage(mDamage);
			mHitEnemies.push_back(enemy);

			mPierce--;
			// If all the pierce was used, destroy the projectile
			if (mPierce == 0)
			{
                toDelete = true;
                return;
			}
		}
	}

	// Update its position
	SetPixelPosition(GetPixelPosition() + mVelocity * mSpeed * Globals::gGame->GetPlayingSpeedDeltaTime());


    // Update lifetime
	mLifetime -= Globals::gGame->GetPlayingSpeedDeltaTime();
}

void Projectile::OnRender()
{
	Point2 pixelPosition(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y);
	Globals::gDrawList->AddCircleFilled(pixelPosition, 5, IM_COL32(0xFF, 0x0, 0x0, 0xFF));

	//Globals::gDrawList->AddLine(pixelPosition, pixelPosition + mVelocity / 2, IM_COL32_BLACK, 3);
}
