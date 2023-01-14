#include "enemy.hpp"
#include "a_star.hpp"
#include "globals.hpp"

Enemy::Enemy(Point2 pixelPosition, uint32_t health, uint8_t damage, uint16_t moneyDrop)
	: Entity(pixelPosition), mHealth(health), mSpawnHealth(health), mDamage(damage), mMoneyDrop(moneyDrop), mSlowed(false)
{
	mCurrentPathIndex = 0;
	mSpeed = 2.f;
}

void Enemy::OnUpdate()
{
	StickToPath();
	SetPixelPosition(GetPixelPosition() + mVelocity * mSpeed * Globals::gGame->GetPlayingSpeedDeltaTime());
}

void Enemy::OnRender()
{
	ImVec2 pos(GetPixelPosition().x + Globals::gGridX, GetPixelPosition().y + Globals::gGridY);
	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, ImVec2(GetScale(), GetScale()), GetRotation());

	// Globals::gDrawList->AddCircleFilled(pos, 20, IM_COL32_BLACK);

	pos.y -= 30.f;
	pos.x -= 30.f;
	Globals::gDrawList->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 60.f, pos.y + 8.f), IM_COL32(0xFF, 0, 0, 0xFF));
	Globals::gDrawList->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + (60.f * mHealth / mSpawnHealth), pos.y + 8), IM_COL32(0, 0xFF, 0, 0xFF));
}

void Enemy::StickToPath()
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

	// Get destination on the path
	ImVec2 dstPos = AStar::recordPositions[mCurrentPathIndex];
	Point2 pixelPos = GetPixelPosition();

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

void Enemy::DealDamage(uint32_t damage)
{
	if (damage >= mHealth)
    {
        toDelete = true;
		Globals::gGame->GetPlayer()->IncreaseMoney(mMoneyDrop);
		return;
	}
	mHealth -= damage;
}
