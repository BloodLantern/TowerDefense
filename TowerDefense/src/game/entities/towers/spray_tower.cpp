#include "spray_tower.hpp"
#include "playfield.hpp"
#include "globals.hpp"
#include "spray_projectile.hpp"

#define CUSTOM_UPGRADE_LEVEL_MAX 4
#define RANDOM_FLOAT(max) (((float_t)rand()/(float_t)(RAND_MAX)) * (max))

static const char* const sCustomUpgradeTooltips[CUSTOM_UPGRADE_LEVEL_MAX] = {
	"Faster spray: the sprays move faster",
	"Intoxicating spray: the spray becomes so toxic that it slows down enemies.",
	"Bigger spray: the sprays become enormous.",
	"More sprays: shoots 3 sprays instead of 1",
};

enum CannonCustomUpgrades
{
	FASTER_SPRAY = 1,
	INTOXICATING_SPRAY = 2,
	MASSIVE_SPRAY = 3,
	MORE_SPRAYS = 4
};

SprayTower::SprayTower(Texture* texture)
	: Tower(new SprayProjectile(), 0.5f, 6.f, "Ant spray", 400, texture)
{
	mScale = .1f;

	mHeight = 2;
	mWidth = 2;
	mProjAmount = 1;

	mCustomUpgradeCost = 250;
	mCustomUpgradeLevelMax = CUSTOM_UPGRADE_LEVEL_MAX;
}

void SprayTower::OnCustomUpgrade()
{
	switch (mCustomUpgradeLevel)
	{
		case FASTER_SPRAY:
			mProjectileTemplate->SetSpeed(5.f);
			mCustomUpgradeCost = 500;
			break;

		case INTOXICATING_SPRAY:
			mCustomUpgradeCost = 750;
			break;

		case MASSIVE_SPRAY:
			mProjectileTemplate->SetHitboxRadius(160.f);
			mProjectileTemplate->SetScale(0.2f);
			mProjectileTemplate->SetDamage(mProjectileTemplate->GetDamage() + 2);
			mCustomUpgradeCost = 1000;
			break;

		case MORE_SPRAYS:
			mProjAmount = 3;
	}
}

const char* const SprayTower::GetCustomUpgradeTooltip(uint8_t level) const
{
	return sCustomUpgradeTooltips[level];
}

void SprayTower::Shoot()
{
	const float_t deltaAngle = .6f;
	for (uint8_t i = 0; i < mProjAmount; i++)
	{
		float_t dangle = deltaAngle - RANDOM_FLOAT(deltaAngle * 2);

		Projectile* proj = mProjectileTemplate->Clone();
		//projTemplate->SetTarget(mTarget);
		Point2 pixelPosition(GetPixelPosition().x + (mWidth * GRID_SQUARE_SIZE / 2.f), GetPixelPosition().y + (mHeight * GRID_SQUARE_SIZE / 2.f) - 20.f);
		proj->SetPixelPosition(pixelPosition);

		Vector2 velocity = Vector2(pixelPosition, mTarget->GetPixelPosition()).Normalize() * 60;

		proj->SetVelocity(velocity.Rotate(dangle));
		proj->SetOwner(this);

		if (mCustomUpgradeLevel >= INTOXICATING_SPRAY)
			proj->SetSlowingEnemies();

		Globals::gGame->projectiles.push_back(proj);
	}
}

Tower* SprayTower::Clone() const
{
	SprayTower* result = new SprayTower(mTexture);
	result->mWidth = mWidth;
	result->mHeight = mHeight;
	return result;
}



#undef CUSTOM_UPGRADE_LEVEL_MAX
