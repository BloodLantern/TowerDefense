#include "bee_projectile.hpp"
#include "globals.hpp"

// Not the best place
#define RANDOM_FLOAT(max) (((float_t)rand()/(float_t)(RAND_MAX)) * (max))

BeeProjectile::BeeProjectile(BeehiveTower* hive)
	: Projectile(5.f, 1.f, 0, INFINITY)
{
	mHive = hive;
	mHookedTimer = 0;
	SetTexture(Globals::gResources->GetTexture("towers\\bee"));
	SetScale(.05f);

	mIdleVelocityTimer = 0.f;
	mOnTarget = false;
	mGoingBackToHive = false;
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

	float_t range = mHive->GetRange() * GRID_SQUARE_SIZE;
	range *= range;
	for (std::vector<Enemy*>::iterator it = Globals::gGame->enemies.begin(); it != Globals::gGame->enemies.end(); ++it)
	{
		Enemy* enemy = *it;

		float_t norm = Vector2(enemy->GetPixelPosition(), mHive->GetPixelPosition()).GetSquaredNorm();
		if (norm < range)
		{
			mTarget = enemy;
			break;
		}
	}

	if (mTarget && !mTarget->toDelete)
	{
		mVelocity = Vector2(GetPixelPosition(), mTarget->GetPixelPosition()).Normalize() * 60;
	}
	else if (!mGoingBackToHive)
	{
		mIdleVelocityTimer -= Globals::gGame->GetPlayingSpeedDeltaTime();
		if (mIdleVelocityTimer < 0)
		{
			mIdleVelocityTimer = RANDOM_FLOAT(3.f);

			mVelocity = Vector2(10.f - RANDOM_FLOAT(20.f), 10.f - RANDOM_FLOAT(20.f));
		}
	}
	else
	{
		if (Vector2(GetPixelPosition(), mHive->GetPixelPosition()).GetSquaredNorm() >= 20.f * 20.f)
			mGoingBackToHive = false;
	}

	HandleEnemyCollision();

	// Update its position
	SetPixelPosition(GetPixelPosition() + mVelocity * mSpeed * Globals::gGame->GetPlayingSpeedDeltaTime());
}

void BeeProjectile::OnRender()
{
	ImVec2 pos(GetPixelPosition().x + Globals::gGridX, GetPixelPosition().y + Globals::gGridY);

	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, ImVec2(GetScale(), GetScale()), GetRotation());
}

void BeeProjectile::HandleEnemyCollision()
{
	if (!mTarget)
		return;

	if (!mOnTarget)
	{
		float_t norm = Vector2(mTarget->GetPixelPosition(), GetPixelPosition()).GetSquaredNorm();
		if (norm < 10.f * 10.f)
			mOnTarget = true;
		return;
	}
	else
	{
		SetPixelPosition(mTarget->GetPixelPosition());
	}

	if (mTarget->toDelete)
	{
		mTarget = nullptr;
		mVelocity = Vector2(GetPixelPosition(), mHive->GetPixelPosition()).Normalize() * 10;
		mGoingBackToHive = true;
		return;
	}
	
	mHookedTimer -= Globals::gGame->GetPlayingSpeedDeltaTime();

	if (mHookedTimer < 0.f)
	{
		mHookedTimer = .8f + RANDOM_FLOAT(.5f);
		uint32_t damageDealt;
		if (mTarget->DealDamage(mDamage, damageDealt))
		{
			mHive->IncreaseKillCount(1);
			mOwner->IncreaseMoneyGenerated(mTarget->GetMoneyDrop());

			mTarget = nullptr;
			mVelocity = Vector2(GetPixelPosition(), mHive->GetPixelPosition()).Normalize() * 10;
			mGoingBackToHive = true;
		}
		mHive->IncreaseDamageDealt(damageDealt);
	}
}

Projectile* BeeProjectile::Clone() const
{
	return new BeeProjectile(mHive);
}
