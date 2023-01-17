#include "cannon_tower.hpp"
#include "cannon_ball_projectile.hpp"
#include "globals.hpp"

CannonTower::CannonTower(Texture* texture)
    : Tower(new CannonBallProjectile(), 0.4f, 5.5f, "Cannon", 25, texture),
	mHandleTexture(Globals::gResources->GetTexture("towers\\cannon_handle")),
	mCannonTexture(Globals::gResources->GetTexture("towers\\cannon"))
{
	mScale = 0.07f;
}

Tower* CannonTower::Clone() const
{
	CannonTower* result = new CannonTower(mTexture);
	result->mWidth = mWidth;
	result->mHeight = mHeight;
	return result;
}

void CannonTower::Draw(const ImVec2 &topLeft, const ImVec2 &bottomRight) const
{
	Globals::gDrawList->AddImage(mHandleTexture->id, topLeft, bottomRight);

	ImVec2 pos = topLeft;
	pos.x += mWidth * GRID_SQUARE_SIZE / 2;
	pos.y += mHeight * GRID_SQUARE_SIZE / 2 - 6;
	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *mCannonTexture, pos, mScale, mRotation);
}
