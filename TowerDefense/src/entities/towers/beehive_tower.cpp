#include "beehive_tower.hpp"
#include "bee_projectile.hpp"
#include "globals.hpp"

BeehiveTower::BeehiveTower(Texture* texture)
	: Tower(new BeeProjectile(this), 3., 7.f, "Beehive", 4, texture)
{
	mMaxAmountOfBees = 5;
	mAmountOfBees = 0;
}

Tower* BeehiveTower::Clone() const
{
	BeehiveTower* result = new BeehiveTower(mTexture);
	result->mWidth = mWidth;
	result->mHeight = mHeight;
	return result;
}

void BeehiveTower::Shoot()
{
	if (mAmountOfBees == mMaxAmountOfBees)
		return;

	mAmountOfBees++;

	Projectile* projTemplate = mProjectileTemplate->Clone();
	projTemplate->SetDamage(mDamage);
	//projTemplate->SetTarget(mTarget);
	Point2 pixelPosition(GetPixelPosition().x + mWidth, GetPixelPosition().y + mHeight);
	projTemplate->SetPixelPosition(pixelPosition);
	projTemplate->SetVelocity(Vector2(pixelPosition, mTarget->GetPixelPosition()).Normalize() * 60);
	projTemplate->SetOwner(this);

	Globals::gGame->projectiles.push_back(projTemplate);
}

void BeehiveTower::RemoveBee()
{
	mAmountOfBees--;
}
