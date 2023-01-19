#include "cannon_tower.hpp"
#include "cannon_ball_projectile.hpp"
#include "cannon_cluster_projectile.hpp"
#include "globals.hpp"

#define CUSTOM_UPGRADE_LEVEL_MAX 4

static const char* const sCustomUpgradeTooltips[CUSTOM_UPGRADE_LEVEL_MAX] = {
	"Faster bombs: never miss again!",
	"Damage increase: now it is also useful in the mid-game.",
	"Cluster bombs: on explosion, the bomb now unleashes 8 other bombs around!\nThe counterpart is less attack speed.",
	"Recursive cluster: cluster bombs now also release more cluster bombs!\nThe counterpart is even less attack speed."
};

enum CannonCustomUpgrades
{
	FASTER_BOMBS = 1,
	DAMAGE_INCREASE = 2,
	CLUSTER_BOMBS = 3,
	RECURSIVE_CLUSTER = 4
};

CannonTower::CannonTower(Texture* texture)
	: Tower(new CannonBallProjectile(), 0.4f, 5.5f, "Cannon", 350, texture),
	mHandleTexture(Globals::gResources->GetTexture("towers\\handle")),
	mCannonTexture(Globals::gResources->GetTexture("towers\\cannon"))
{
	mScale = 0.07f;

	mCustomUpgradeCost = 150;
	mCustomUpgradeLevelMax = CUSTOM_UPGRADE_LEVEL_MAX;
}

void CannonTower::OnCustomUpgrade()
{
	switch (mCustomUpgradeLevel)
	{
		case FASTER_BOMBS:
			mProjectileTemplate->SetSpeed(30);

			mCustomUpgradeCost = 200;
            break;
		case DAMAGE_INCREASE:
		    mDamage = 5;

			mCustomUpgradeCost = 1500;
            break;
		case CLUSTER_BOMBS:
			delete mProjectileTemplate;
			mProjectileTemplate = new CannonClusterProjectile(false);
			mProjectileTemplate->SetOwner(this);
			mProjectileTemplate->SetPierce(80);
			mAttackSpeed -= 0.1f;

			mCustomUpgradeCost = 3000;
            break;
		case RECURSIVE_CLUSTER:
			delete mProjectileTemplate;
			mProjectileTemplate = new CannonClusterProjectile(true);
			mProjectileTemplate->SetOwner(this);
			mProjectileTemplate->SetPierce(500);
			mAttackSpeed -= 0.2f;
            break;
	}
}

const char* const CannonTower::GetCustomUpgradeTooltip(uint8_t level) const
{
    return sCustomUpgradeTooltips[level];
}

Tower* CannonTower::Clone() const
{
	return new CannonTower(mTexture);
}

void CannonTower::Draw(const ImVec2 &topLeft, const ImVec2 &bottomRight) const
{
	Globals::gDrawList->AddImage(mHandleTexture->id, topLeft, bottomRight);

	ImVec2 pos = topLeft;
	pos.x += mWidth * GRID_SQUARE_SIZE / 2;
	pos.y += mHeight * GRID_SQUARE_SIZE / 2 - 6;
	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *mCannonTexture, pos, mScale, mRotation);
}
