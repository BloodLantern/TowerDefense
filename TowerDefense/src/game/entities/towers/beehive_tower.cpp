#include "beehive_tower.hpp"
#include "globals.hpp"
#include "beenest_tower.hpp"

#define CUSTOM_UPGRADE_LEVEL_MAX 4

static const char* const sCustomUpgradeTooltips[CUSTOM_UPGRADE_LEVEL_MAX] = {
	"Faster production: bees become even more productive.",
	"High quality honey: produces high quality honey that sells for more.",
	"Healthy honey: honey so healthy it regenerates life sometimes.",
	"Friendly bees: boosts the beenests in the radius",
};

enum BeehiveCustomUpgrades
{
	FASTER_PRODUCTION = 1,
	HIGH_QUALITY_HONEY = 2,
	HEALTHY_HONEY = 3,
	FRIENDLY_BEES = 4
};


BeehiveTower::BeehiveTower(Texture* texture)
	: Tower(2.f, 8.f, "Beehive", 800, texture)
{
	mCashBonus = 25;
	mMaxCashGenerations = 8;
	mAmoutOfCashGenerated = 0;
	mCashGenerationTimer = mAttackSpeed;

	mCustomUpgradeLevelMax = CUSTOM_UPGRADE_LEVEL_MAX;
	mCustomUpgradeCost = 500;

	mWidth = 3;
	mHeight = 3;
}

void BeehiveTower::OnCustomUpgrade()
{
	switch (mCustomUpgradeLevel)
	{
		case FASTER_PRODUCTION:
			mStartAttackSpeed += .5f;
			UpdateAttackSpeed();
			mCustomUpgradeCost = 750;
			break;

		case HIGH_QUALITY_HONEY:
			mCashBonus = 50;
			mMaxCashGenerations = 15;
			mCustomUpgradeCost = 2000;
			break;

		case HEALTHY_HONEY:
			mCustomUpgradeCost = 1000;
			break;

		case FRIENDLY_BEES:
			break;
	}
}

const char* const BeehiveTower::GetCustomUpgradeTooltip(uint8_t level) const
{
	return sCustomUpgradeTooltips[level];
}

void BeehiveTower::OnUpdate()
{
	if (Globals::gGame->IsFirstFrameOfRound())
	{
		mAmoutOfCashGenerated = 0; // Reset cash counter

		mCashGenerationTimer = mAttackSpeed;
	}

	// Check for selection
	HandleSelection();

	TryBuffBeenests();

	if (mAmoutOfCashGenerated == mMaxCashGenerations)
		return;

	
	mCashGenerationTimer -= Globals::gGame->GetPlayingSpeedDeltaTime();

	if (mCashGenerationTimer < 0)
	{
		Player* player = Globals::gGame->GetPlayer();
		mCashGenerationTimer = mAttackSpeed;
		mAmoutOfCashGenerated++;

		player->IncreaseMoney(mCashBonus);
		IncreaseMoneyGenerated(mCashBonus);

		if (mCustomUpgradeLevel < HEALTHY_HONEY)
			return;

		if ((rand() % 100) <= 10)
			player->IncreaseLife(rand() % 3);
	}
}

void BeehiveTower::Shoot()
{
	// Don't do anything
	return;
}

Tower* BeehiveTower::Clone() const
{
	BeehiveTower* result = new BeehiveTower(mTexture);
	result->mWidth = mWidth;
	result->mHeight = mHeight;
	return result;
}

void BeehiveTower::TryBuffBeenests()
{
	if (mCustomUpgradeLevel < FRIENDLY_BEES)
		return;

	std::vector<Tower*>* towers = Globals::gGame->GetPlayer()->GetTowers();
	float_t range = mRange * GRID_SQUARE_SIZE;
	range *= range;

	for (std::vector<Tower*>::iterator _t = towers->begin(); _t != towers->end(); _t++)
	{
		Tower* t = *_t;

		// This violates the Liskow substitution principle (https://en.wikipedia.org/wiki/Liskov_substitution_principle)
		// according to people on stack overflow (https://stackoverflow.com/questions/307765/how-do-i-check-if-an-objects-type-is-a-particular-subclass-in-c)
		// don't really see another way to do that though and it shouldn't be an issue anyway, just though it was worth mentionning
		
		BeenestTower* nest = dynamic_cast<BeenestTower*>(t);
		if (nest == nullptr) // Check is a beenest
			continue;

		// Check isn't already buffed
		if (nest->IsBuffed())
			continue;

		float_t dist = Vector2(GetPixelPosition(), nest->GetPixelPosition()).GetSquaredNorm();

		if (dist < range)
			nest->ApplyBeehiveBuff();
	}
}
