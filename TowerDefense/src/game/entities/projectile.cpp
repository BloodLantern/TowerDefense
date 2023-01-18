#include "projectile.hpp"
#include "globals.hpp"

Projectile::Projectile(float_t speed, uint32_t damage, uint16_t pierce, float_t lifetime)
    : Entity(), mVelocity(Vector2(0, 0)), mSpeed(speed), mDamage(damage), mPierce(pierce), mLifetime(lifetime)
{
}

Projectile* Projectile::Clone() const
{
    return new Projectile(mSpeed, mDamage, mPierce, mLifetime);
}

void Projectile::OnUpdate()
{
	// If the projectile lifetime is over, destroy it
	if (mLifetime <= 0.f)
	{
		toDelete = true;
		return;
	}

	// Update velocity if the projectile has a target
	if (mTarget && !mTarget->toDelete)
		mVelocity = Vector2(GetPixelPosition(), mTarget->GetPixelPosition()).Normalize() * 60;

	// If the projectile hits an enemy, deal its damage and reduce its pierce
	HandleEnemyCollision();

	// Update its position
	SetPixelPosition(GetPixelPosition() + mVelocity * mSpeed * Globals::gGame->GetPlayingSpeedDeltaTime());


    // Update lifetime
	mLifetime -= Globals::gGame->GetPlayingSpeedDeltaTime();
}

void Projectile::HandleEnemyCollision()
{
	// For each enemy
	for (std::vector<Enemy*>::iterator it = Globals::gGame->enemies.begin(); it != Globals::gGame->enemies.end(); ++it)
	{
		Enemy* enemy = *it;
		// If the enemy hasn't been hit already
		if (std::find(mHitEnemies.begin(), mHitEnemies.end(), enemy) != mHitEnemies.end())
			continue;

		// If it collides
		if (Vector2(enemy->GetPixelPosition(), GetPixelPosition()).GetSquaredNorm() < PROJECTILE_COLLISION_RADIUS * PROJECTILE_COLLISION_RADIUS)
		{
			uint32_t damageDealt = 0;
			// If the enemy died, update tower kill stat
			if (!enemy->toDelete && enemy->DealDamage(mDamage, damageDealt))
			{
				if (mOwner)
				{
					mOwner->IncreaseKillCount(1);
					mOwner->IncreaseMoneyGenerated(enemy->GetMoneyDrop());
				}
			}
			// Otherwise, add it to the hit enemies to avoid hitting it multiple times
			else
			{
				mHitEnemies.push_back(enemy);
			}
			if (mOwner)
				mOwner->IncreaseDamageDealt(damageDealt);

			// If all the pierce was used, destroy the projectile
			if (mPierce == 0)
			{
                toDelete = true;
                return;
			}
			mPierce--;
		}
	}
}

void Projectile::OnRender()
{
	Point2 pixelPosition(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y);
	Globals::gDrawList->AddCircleFilled(pixelPosition, 5, IM_COL32(0xFF, 0x0, 0x0, 0xFF));

	//Globals::gDrawList->AddLine(pixelPosition, pixelPosition + mVelocity / 2, IM_COL32_BLACK, 3);
}
