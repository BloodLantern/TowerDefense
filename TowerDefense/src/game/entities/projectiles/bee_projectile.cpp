#include "bee_projectile.hpp"
#include "globals.hpp"
#include "beehive_tower.hpp"
#include "bee_projectile_dart.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

// Not the best place
#define RANDOM_FLOAT(max) (((float_t)rand()/(float_t)(RAND_MAX)) * (max))

BeeProjectile::BeeProjectile(BeehiveTower* hive)
	: Projectile(5.f, 1.f, 0, INFINITY)
{
	mHive = hive;
	mHookedTimer = 0;
	mDartTimer = 1.f;
	slowEnemies = false;
	SetTexture(Globals::gResources->GetTexture("towers\\bee_right"));
	SetScale(.05f);

	mIdleVelocityTimer = 0.f;
	mPose = BEE_POSE_IDLE;
}

BeeProjectile::~BeeProjectile()
{
	if (!toDelete)
		mHive->RemoveBee(this);
}

Projectile* BeeProjectile::Clone() const
{
	return new BeeProjectile(mHive);
}

void BeeProjectile::UpdateForDarts()
{
	if (!mTarget)
		return;
	
	mDartTimer -= Globals::gGame->GetPlayingSpeedDeltaTime();
	if (mDartTimer < 0)
	{
		mDartTimer = 1.f;

		Vector2 velocity = Vector2(GetPixelPosition(), mTarget->GetPixelPosition()) * 60;

		// Don't use the projectile queue because we aren't in the projectile update loop
		Globals::gGame->projectiles.push_back(new BeeProjectileDart(GetPixelPosition(), velocity));
	}
}

void BeeProjectile::OnUpdate()
{
	// Safety net, if the beehive was destroyed earlier in the frame
	if (toDelete)
		return;

	switch (mPose)
	{
		case BEE_POSE_IDLE:
			HandleIdleFloating();
			FindTarget();
			break;

		case BEE_POSE_BACK_TO_HIVE:
			if (CheckAtHive())
				mPose = BEE_POSE_IDLE;
			FindTarget();
			break;

		case BEE_POSE_GOING_TO_TARGET:
			if (mTarget->toDelete)
			{
				// Target is dead, go back to hive
				SetGoingBackToHive();
				break;
			}

			// Set velocity to go to the target fairly fast
			mVelocity = Vector2(GetPixelPosition(), mTarget->GetPixelPosition()).Normalize() * 60;
			HandleGoingToTarget();
			break;

		case BEE_POSE_ON_TARGET:
			if (mTarget->toDelete)
			{
				// Target is dead, go back to hive
				SetGoingBackToHive();
				break;
			}

			HandleEnemyCollision();
	}

	MakeFaceDirection();

	// Update position
	SetPixelPosition(GetPixelPosition() + mVelocity * mSpeed * Globals::gGame->GetPlayingSpeedDeltaTime());
	// Always stay in hive range

	if (!CheckInHiveRange())
		SetGoingBackToHive();
}

void BeeProjectile::OnRender()
{
	ImVec2 pos(GetPixelPosition().x + Globals::gGridX, GetPixelPosition().y + Globals::gGridY);

	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, mScale, mRotation);
}

void BeeProjectile::MakeFaceDirection()
{
	if (mVelocity.x < 0)
		mTexture = Globals::gResources->GetTexture("towers\\bee_left");
	else
		mTexture = Globals::gResources->GetTexture("towers\\bee_right");
}

void BeeProjectile::HandleEnemyCollision()
{
	// Stick to target
	SetPixelPosition(mTarget->GetPixelPosition());
	
	// Update timer
	mHookedTimer -= Globals::gGame->GetPlayingSpeedDeltaTime();

	if (mHookedTimer < 0.f)
	{
		// Set random timer for next damage ([.8;1.3] s)
		mHookedTimer = .8f + RANDOM_FLOAT(.5f);

		// Deal damage and update tower
		uint32_t damageDealt;

		if (slowEnemies)
			mTarget->SlowDown();

		if (mTarget->DealDamage(mDamage, damageDealt))
		{
			mHive->IncreaseKillCount(1);
			mOwner->IncreaseMoneyGenerated(mTarget->GetMoneyDrop());

			// Go back to hive
			SetGoingBackToHive();
		}
		mHive->IncreaseDamageDealt(damageDealt);
	}
}

bool BeeProjectile::CheckInHiveRange()
{
	// Get pixel range of the hive
	float_t range = mHive->GetRange() * GRID_SQUARE_SIZE;

	// Check bee still in range
	return Vector2(GetPixelPosition(), mHive->GetPixelPosition()).GetSquaredNorm() < range * range;
}

bool BeeProjectile::CheckAtHive()
{
	Point2 dst = mHive->GetPixelPosition();

	// Get center position
	dst.x += mHive->GetWidth() / 2.f * GRID_SQUARE_SIZE;
	dst.y += mHive->GetHeight() / 2.f * GRID_SQUARE_SIZE;

	// TODO use hive hitbox instead of hardcoded 20 pixels
	return Vector2(GetPixelPosition(), dst).GetSquaredNorm() < 20.f * 20.f;
}


void BeeProjectile::FindTarget()
{
	// Get pixel range of the hive
	float_t range = mHive->GetRange() * GRID_SQUARE_SIZE;

	// Square range for optimisation (avoid sqrt call)
	range *= range;

	// Loop the enemies
	for (std::vector<Enemy*>::iterator it = Globals::gGame->enemies.begin(); it != Globals::gGame->enemies.end(); ++it)
	{
		Enemy* enemy = *it;

		// Get squared distance
		float_t norm = Vector2(enemy->GetPixelPosition(), mHive->GetPixelPosition()).GetSquaredNorm();
		if (norm < range)
		{
			// Found enemy in range, set target
			mTarget = enemy;
			mPose = BEE_POSE_GOING_TO_TARGET;
			break;
		}
	}
}

void BeeProjectile::HandleIdleFloating()
{
	// Check timer
	mIdleVelocityTimer -= Globals::gGame->GetPlayingSpeedDeltaTime();
	if (mIdleVelocityTimer < 0)
	{
		// Set random timer to change direction
		mIdleVelocityTimer = RANDOM_FLOAT(3.f);

		// Set random velocity ([-10;+10] px/s in any direction)
		mVelocity = Vector2(10.f - RANDOM_FLOAT(20.f), 10.f - RANDOM_FLOAT(20.f));
	}
}

void BeeProjectile::SetGoingBackToHive()
{
	// Set no target
	mTarget = nullptr;

	Point2 dst = mHive->GetPixelPosition();

	// Get center position
	dst.x += mHive->GetWidth() / 2.f * GRID_SQUARE_SIZE;
	dst.y += mHive->GetHeight() / 2.f * GRID_SQUARE_SIZE;

	// Set velocity to slowly go back to hive
	mVelocity = Vector2(GetPixelPosition(), dst).Normalize() * 20;

	mPose = BEE_POSE_BACK_TO_HIVE;
}

void BeeProjectile::HandleGoingToTarget()
{
	float_t norm = Vector2(mTarget->GetPixelPosition(), GetPixelPosition()).GetSquaredNorm();
	if (norm < 10.f * 10.f)
		mPose = BEE_POSE_ON_TARGET;
}

#undef RANDOM_FLOAT
