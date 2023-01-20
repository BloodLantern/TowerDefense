#include "beenest_tower.hpp"
#include "globals.hpp"

#define CUSTOM_UPGRADE_LEVEL_MAX 4

enum BeenestCustomUpgrades
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

BeenestTower::BeenestTower(Texture* texture)
	: Tower(new BeeProjectile(this), 0.33f, 7.f, "Beenest", 500, texture)
{
	mDamage = 5;

	mMaxAmountOfBees = 5;
	mAmountOfBees = 0;

	mWidth = 2;
	mHeight = 2;

	mRotateTowardsEnemies = false;

	mCustomUpgradeCost = 250;

	mCustomUpgradeLevelMax = CUSTOM_UPGRADE_LEVEL_MAX;

	mBuffed = false;
	mBuffParticlesEmitter = nullptr;
}

BeenestTower::~BeenestTower()
{
	for (size_t i = 0; i < mBees.size(); i++)
		mBees[i]->toDelete = true;

	if (mBuffParticlesEmitter)
		delete mBuffParticlesEmitter;
}

Tower* BeenestTower::Clone() const
{
	BeenestTower* result = new BeenestTower(mTexture);
	result->mWidth = mWidth;
	result->mHeight = mHeight;
	return result;
}

void BeenestTower::OnUpdate()
{
	// Check for darts
	if (mCustomUpgradeLevel >= DARTS)
	{
		for (size_t i = 0; i < mBees.size(); i++)
			mBees[i]->UpdateForDarts();
	}

	// Check for selection
	HandleSelection();

	if (mBuffed)
	{
		mDamage++;
		mBuffParticlesEmitter->Start();
		mBuffParticlesEmitter->Update();
	}

	// Check whether the tower is able to shoot
	if (mTimeSinceLastAttack > 1 / GetAttackSpeed())
		Shoot();

	if (mBuffed)
	{
		mDamage--;
		mBuffParticlesEmitter->Stop();
		mBuffed = false;
	}

	// Update attack cooldown
	mTimeSinceLastAttack += Globals::gGame->GetPlayingSpeedDeltaTime();
}

void BeenestTower::OnCustomUpgrade()
{
	switch (mCustomUpgradeLevel)
	{
		case MORE_BEES:
			mMaxAmountOfBees = 8;
			mCustomUpgradeCost = 500;
			break;

		case DARTS:
			mCustomUpgradeCost = 1000;
			break;

		case STICKY_HONEY:
			mCustomUpgradeCost = 1500;
			break;

		case ANGRY_BEES:
			mDamage += 3;
			break;
	}
}

const char* const BeenestTower::GetCustomUpgradeTooltip(uint8_t level) const
{
	return sCustomUpgradeTooltips[level];
}

void BeenestTower::Shoot()
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

void BeenestTower::RemoveBee(BeeProjectile* bee)
{
	if (mAmountOfBees == 0)
		return;
	mAmountOfBees--;
	mBees.erase(std::find(mBees.begin(), mBees.end(), bee));
}

bool BeenestTower::IsBuffed()
{
	return mBuffed;
}

void BeenestTower::ApplyBeehiveBuff()
{
	mBuffed = true;
	if (!mBuffParticlesEmitter)
	{
		Point2 position = GetPixelPosition();
		position.x += (mWidth * GRID_SQUARE_SIZE) / 2.f;
		position.y += (mHeight * GRID_SQUARE_SIZE) / 2.f;

		mBuffParticlesEmitter = new ParticleEmitter(position, *Globals::gResources->GetTexture("particles\\buff"));
	}
	mBuffParticlesEmitter->ConfigureEmitter(3, 0.5f, 0.1f, 0.0f, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), Vector2(0, -10), 200.f, 5.f);
}


#undef CUSTOM_UPGRADE_LEVEL_MAX
