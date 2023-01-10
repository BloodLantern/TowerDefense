#include "Enemy.h"
#include "AStar.h"
#include "Globals.h"

Enemy::Enemy(Point2 pixelPosition, uint32_t health, uint8_t damage, uint16_t moneyDrop)
	: Entity(pixelPosition), mHealth(health), mDamage(damage), mMoneyDrop(moneyDrop), mSlowed(false)
{
}

void Enemy::OnUpdate()
{

}

void Enemy::OnRender()
{
	SetPixelPosition(GetPixelPosition() + mVelocity);

	ImVec2 pos(GetPixelPosition().x + Globals::gGridX, GetPixelPosition().y + Globals::gGridY);
	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, ImVec2(GetScale(), GetScale()), GetRotation());
}

void Enemy::StickToPath()
{
	if (AStar::recordPositions.size() <= 0)
	{
		PlayField* pf = Globals::gGame->GetPlayField();
		// Failsafe
		AStar::FindBestPath(pf->mGridWidth - 1, pf->mGridHeight / 2, 0, pf->mGridHeight / 2);
		return;
	}

	if (mCurrentPathIndex >= AStar::recordPositions.size())
	{
		// Ended path, stop moving
		mVelocity = Vector2(0, 0);
		return;
	}

	// Get destination on the path
	ImVec2 dstPos = AStar::recordPositions[mCurrentPathIndex];
	Point2 pixelPos = GetPixelPosition();

	// Check is on the destination tile
	if (fabsf(pixelPos.x - dstPos.x) < 1.f && fabsf(pixelPos.y - dstPos.y) < 1.f)
	{
		// Advance path
		mCurrentPathIndex++;
		return;
	}

	// Move to path at standard speed
	mVelocity = Vector2(pixelPos, Point2(dstPos.x, dstPos.y)).Normalize();
}
