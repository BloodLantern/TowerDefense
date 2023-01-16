#include "beehive_tower.hpp"
#include "bee_projectile.hpp"
#include "globals.hpp"

BeehiveTower::BeehiveTower(Texture* texture)
	: Tower(new BeeProjectile(this), 0.33f, 7.f, "Beehive", 4, texture)
{
	mMaxAmountOfBees = 5;
	mAmountOfBees = 0;

	mWidth = 2;
	mHeight = 2;
}

Tower* BeehiveTower::Clone() const
{
	BeehiveTower* result = new BeehiveTower(mTexture);
	result->mWidth = mWidth;
	result->mHeight = mHeight;
	return result;
}

void BeehiveTower::OnUpdate()
{
	// Check for selection
	HandleSelection();

	// Check whether the tower is able to shoot
	if (mTimeSinceLastAttack > 1 / GetAttackSpeed())
		Shoot();

	// Update attack cooldown
	mTimeSinceLastAttack += Globals::gGame->GetPlayingSpeedDeltaTime();
}

void BeehiveTower::Shoot()
{
	mTimeSinceLastAttack = 0;
	if (mAmountOfBees == mMaxAmountOfBees)
		return;

	mAmountOfBees++;

	Projectile* projTemplate = mProjectileTemplate->Clone();
	projTemplate->SetDamage(mDamage);
	//projTemplate->SetTarget(mTarget);
	Point2 pixelPosition(GetPixelPosition().x + (mWidth * GRID_SQUARE_SIZE) / 2.f,
		GetPixelPosition().y + (mHeight * GRID_SQUARE_SIZE) / 2.f);
	projTemplate->SetPixelPosition(pixelPosition);
	projTemplate->SetVelocity(Vector2(0, 0));
	projTemplate->SetOwner(this);

	Globals::gGame->projectiles.push_back(projTemplate);
}

void BeehiveTower::RemoveBee()
{
	mAmountOfBees--;
}
