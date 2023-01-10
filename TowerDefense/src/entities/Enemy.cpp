#include "Enemy.h"
#include "AStar.h"
#include "Globals.h"

Enemy::Enemy(Point2 pixelPosition, uint32_t health, uint8_t damage, uint16_t moneyDrop)
	: Entity(pixelPosition), mHealth(health), mDamage(damage), mMoneyDrop(moneyDrop), mSlowed(false)
{
}

void Enemy::Draw()
{
	SetPixelPosition(GetPixelPosition() + mVelocity);

	ImVec2 pos(GetPixelPosition().x + Globals::gGridX, GetPixelPosition().y + Globals::gGridY);
	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, ImVec2(GetScale(), GetScale()), GetRotation());
}

void Enemy::StickToPath()
{
	if (AStar::recordPositions.size() <= 0)
		AStar::FindBestPath(Globals::gGame->GetPlayField()->mGridWidth - 1, Globals::gGame->GetPlayField()->mGridHeight / 2, 0, Globals::gGame->GetPlayField()->mGridHeight / 2);

	if (mCurrentPathIndex >= AStar::recordPositions.size())
	{
		mVelocity = Vector2(0, 0);
		return;
	}

	PlayField* pf = Globals::gGame->GetPlayField();
	ImVec2 pos = AStar::recordPositions[mCurrentPathIndex];

	mVelocity = Vector2(GetPixelPosition(), Point2(pos.x, pos.y)).Normalize();

	pf->GetGridPositionFromPixels(pos.x, pos.y, mGridDestinationX, mGridDestinationY);
	Point2 dstPos(pos.x, pos.y);

	Point2 pixelPos = GetPixelPosition();
	if (fabsf(pixelPos.x - dstPos.x) < 1.f && fabsf(pixelPos.y - dstPos.y) < 1.f)
		mCurrentPathIndex++;
}
