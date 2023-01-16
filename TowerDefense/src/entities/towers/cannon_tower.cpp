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

void CannonTower::Shoot()
{
    CannonBallProjectile* projTemplate = new CannonBallProjectile(*static_cast<CannonBallProjectile*>(mProjectileTemplate));
	projTemplate->SetDamage(mDamage);
	//projTemplate->SetTarget(mTarget);
	Point2 pixelPosition(GetPixelPosition().x + mWidth, GetPixelPosition().y + mHeight);
	projTemplate->SetPixelPosition(pixelPosition);
	projTemplate->SetVelocity(Vector2(pixelPosition, mTarget->GetPixelPosition()).Normalize() * 60);
	projTemplate->SetOwner(this);

	Globals::gGame->projectiles.push_back(projTemplate);
}
