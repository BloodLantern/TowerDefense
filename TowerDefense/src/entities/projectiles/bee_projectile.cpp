#include "bee_projectile.hpp"
#include "globals.hpp"

BeeProjectile::BeeProjectile(BeehiveTower* hive)
	: Projectile(3.f, 1.f, 0, INFINITY)
{
	mHive = hive;
	mHookedTimer = 0;
}

BeeProjectile::~BeeProjectile()
{
	mHive->RemoveBee();
}

void BeeProjectile::OnUpdate()
{
	if (mTarget)
	{
		float_t range = mHive->GetRange();
		if (Vector2(mTarget->GetPixelPosition(), mHive->GetPixelPosition()).GetSquaredNorm() >= range * range)
		{
			mTarget = nullptr;

		}
		return;
	}

	float_t range = mHive->GetRange();
	for (std::vector<Enemy*>::iterator it = Globals::gGame->enemies.begin(); it != Globals::gGame->enemies.end(); ++it)
	{
		Enemy* enemy = *it;

		float_t norm = Vector2(enemy->GetPixelPosition(), mHive->GetPixelPosition()).GetSquaredNorm();
		if (norm < range * range * GRID_SQUARE_SIZE)
		{
			mTarget = enemy;
			break;
		}
	}

	Projectile::OnUpdate();
}

void BeeProjectile::HandleEnemyCollision()
{
	if (!mTarget)
		return;

	mHookedTimer += Globals::gGame->GetPlayingSpeedDeltaTime();

	if (mHookedTimer > 1.f)
	{
		mHookedTimer = 0;
		uint32_t damageDealt;
		if (mTarget->DealDamage(mDamage, damageDealt))
		{
			mHive->IncreaseKillCount(1);
			mOwner->IncreaseMoneyGenerated(mTarget->GetMoneyDrop());

			mTarget = nullptr;
		}
		mHive->IncreaseDamageDealt(damageDealt);
	}
}

Projectile* BeeProjectile::Clone() const
{
	return new BeeProjectile(mHive);
}
