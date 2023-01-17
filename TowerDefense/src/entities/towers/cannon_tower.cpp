#include "cannon_tower.hpp"
#include "cannon_ball_projectile.hpp"
#include "globals.hpp"

CannonTower::CannonTower(Texture* texture)
    : Tower(new CannonBallProjectile(), 0.4f, 5.5f, "Cannon", 250, texture)
{
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
	Globals::gDrawList->AddImage(GetTexture()->id, topLeft, bottomRight);
}
