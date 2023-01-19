#include "beehive_tower.hpp"
#include "globals.hpp"

#define CUSTOM_UPGRADE_LEVEL_MAX 4

enum BeehiveCustomUpgrades
{
	MORE_BEES = 1,
	DARTS = 2,
	STICKY_HONEY = 3,
	ANGRY_BEES = 4
};

static const char* const sCustomUpgradeTooltips[CUSTOM_UPGRADE_LEVEL_MAX] = {
	"More bees: Increases the amount of bees the hive can spawn up to 8",
	"Darts: Bees will now shoot darts at their enemies",
	"Sticky honey: Enemies attacked by the bees will be slowed by honey",
	"Angry bees: Increases the damage dealt by bees"
};

BeehiveTower::BeehiveTower(Texture* texture)
	: Tower(new BeeProjectile(this), 0.33f, 7.f, "Beehive", 500, texture)
{
	mDamage = 5;

	mMaxAmountOfBees = 5;
	mAmountOfBees = 0;

	mWidth = 2;
	mHeight = 2;

	mRotateTowardsEnemies = false;

	mCustomUpgradeCost = 1000;

	mCustomUpgradeLevelMax = CUSTOM_UPGRADE_LEVEL_MAX;
}

BeehiveTower::~BeehiveTower()
{
	for (size_t i = 0; i < mBees.size(); i++)
		mBees[i]->toDelete = true;
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
	// Check for darts
	if (mCustomUpgradeLevel >= DARTS)
	{
		for (size_t i = 0; i < mBees.size(); i++)
			mBees[i]->UpdateForDarts();
	}

	// Check for selection
	HandleSelection();

	// Check whether the tower is able to shoot
	if (mTimeSinceLastAttack > 1 / GetAttackSpeed())
		Shoot();

	// Update attack cooldown
	mTimeSinceLastAttack += Globals::gGame->GetPlayingSpeedDeltaTime();
}

void BeehiveTower::OnCustomUpgrade()
{
	switch (mCustomUpgradeLevel)
	{
		case MORE_BEES:
			mMaxAmountOfBees = 8;
			mCustomUpgradeCost = 500;
			break;

		case DARTS:
			mCustomUpgradeCost = 2000;
			break;

		case ANGRY_BEES:
			mDamage += 3;
			mCustomUpgradeCost = 5000;
			break;
	}
}

const char* const BeehiveTower::GetCustomUpgradeTooltip(uint8_t level) const
{
	return sCustomUpgradeTooltips[level];
}

void BeehiveTower::Shoot()
{
	mTimeSinceLastAttack = 0;
	if (mAmountOfBees == mMaxAmountOfBees)
		return;

	mAmountOfBees++;

	Projectile* projTemplate = mProjectileTemplate->Clone();
	projTemplate->SetDamage(mDamage);

	Point2 pixelPosition(GetPixelPosition().x + (mWidth * GRID_SQUARE_SIZE) / 2.f,
		GetPixelPosition().y + (mHeight * GRID_SQUARE_SIZE) / 2.f);
	projTemplate->SetPixelPosition(pixelPosition);

	projTemplate->SetVelocity(Vector2(0, 0));
	projTemplate->SetOwner(this);

	Globals::gGame->projectiles.push_back(projTemplate);

	BeeProjectile* bee = dynamic_cast<BeeProjectile*>(projTemplate);
	
	if (mCustomUpgradeLevel >= STICKY_HONEY)
		bee->slowEnemies = true;

	mBees.push_back(bee);
}

void BeehiveTower::RemoveBee(BeeProjectile* bee)
{
	mAmountOfBees--;
	mBees.erase(std::find(mBees.begin(), mBees.end(), bee));
}

#undef CUSTOM_UPGRADE_LEVEL_MAX
