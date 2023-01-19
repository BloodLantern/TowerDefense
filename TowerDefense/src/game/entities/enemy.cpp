#include "enemy.hpp"
#include "a_star.hpp"
#include "globals.hpp"

Enemy::Enemy(Point2 pixelPosition, uint32_t health, uint16_t damage, uint32_t moneyDrop)
	: Entity(pixelPosition), mHealth(health), mSpawnHealth(health), mDamage(damage), mMoneyDrop(moneyDrop), mSlowed(false)
{
	mCurrentPathIndex = 0;
	mSpeed = 1.f;
	mSlowedTimer = 0.f;
}

void Enemy::OnUpdate()
{
	StickToPath();

	float_t deltaTime = Globals::gGame->GetPlayingSpeedDeltaTime();


	float_t speedModifier = 1.f;
	if (mSlowed)
	{
		speedModifier = 0.5f;
		mSlowedTimer -= deltaTime;
		if (mSlowedTimer < 0)
		{
			float_t speedModifier = 1.f;
			mSlowed = false;
		}
	}

	SetPixelPosition(GetPixelPosition() + mVelocity * mSpeed * deltaTime * speedModifier);

	if (mRotateForward)
		SetRotation(mVelocity.Angle());
}

void Enemy::OnRender()
{
	ImVec2 pos(GetPixelPosition().x + Globals::gGridX, GetPixelPosition().y + Globals::gGridY);

	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, mScale, mRotation);

	// Globals::gDrawList->AddCircleFilled(pos, 20, IM_COL32_BLACK);

	DrawHealthBar(pos);
}

void Enemy::StickToPath()
{
	ImVec2 dstPos;
	Point2 pixelPos = GetPixelPosition();

	// Get destination on the path
	if (0)
	{
		if (AStar::recordPositions.size() <= 0)
		{
			PlayField* pf = Globals::gGame->GetPlayField();
			// Failsafe
			AStar::FindBestPath(pf->gridWidth - 1, pf->gridHeight / 2, 0, pf->gridHeight / 2);
			return;
		}

		if (mCurrentPathIndex >= AStar::recordPositions.size())
		{
			// Ended path, keep previous velocity to "walk off screen"
			if (!IsOnGrid())
			{
				// Deal damage
				Globals::gGame->GetPlayer()->DecreaseLife(mDamage);
				// Flag for deletion
				toDelete = true;
			}
			return;
		}
		dstPos = AStar::recordPositions[mCurrentPathIndex];
	}
	else
	{
		PlayField* pf = Globals::gGame->GetPlayField();

		if (mCurrentPathIndex >= pf->GetPathNodes().size())
		{
			// Ended path, keep previous velocity to "walk off screen"
			if (!IsOnGrid())
			{
				// Deal damage
				Globals::gGame->GetPlayer()->DecreaseLife(mDamage);
				// Flag for deletion
				toDelete = true;
			}
			return;
		}
		PathNode& node = pf->GetPathNodes()[mCurrentPathIndex];

		dstPos = ImVec2(node.x * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2.f,
			node.y * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2.f);
	}

	// Check is on the destination tile
	if (fabsf(pixelPos.x - dstPos.x) < 10.f && fabsf(pixelPos.y - dstPos.y) < 10.f)
	{
		// Advance path
		mCurrentPathIndex++;
		return;
	}

	// Move to path at standard speed
	mVelocity = Vector2(pixelPos, Point2(dstPos.x, dstPos.y)).Normalize() * 60;
}

bool Enemy::DealDamage(uint32_t damage, uint32_t& damageDealt)
{
	if (damage >= mHealth)
    {
		damageDealt = mHealth;
        toDelete = true;
		Globals::gGame->GetPlayer()->IncreaseMoney(mMoneyDrop);
		return true;
	}
	
	mHealth -= damage;
	damageDealt = damage;
	return false;
}

bool Enemy::Heal(uint32_t increment)
{
	int32_t rest = mSpawnHealth - mHealth;
	if (increment >= rest)
	{
		mHealth = mSpawnHealth;
		return true;
	}

	mHealth += increment;
	return false;
}

void Enemy::SlowDown()
{
	mSlowed = true;
	mSlowedTimer = 2.5f;
}

void Enemy::DrawHealthBar(ImVec2& pos)
{
	if (mHealth == mSpawnHealth)
		return;

	pos.y -= 30.f;
	pos.x -= 30.f;
	Globals::gDrawList->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 60.f, pos.y + 8.f), IM_COL32(0xFF, 0, 0, 0xFF));
	Globals::gDrawList->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + (60.f * mHealth / mSpawnHealth), pos.y + 8), IM_COL32(0, 0xFF, 0, 0xFF));
}
