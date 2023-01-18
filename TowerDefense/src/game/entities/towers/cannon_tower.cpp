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

CannonTower::CannonTower(Texture* texture)
	: Tower(new CannonBallProjectile(), 0.4f, 5.5f, "Cannon", 350, texture),
	mHandleTexture(Globals::gResources->GetTexture("towers\\cannon_handle")),
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
		case 1:
			mProjectileTemplate->SetSpeed(30);

			mCustomUpgradeCost = 200;
            break;
		case 2:
		    mDamage += 1;

			mCustomUpgradeCost = 1500;
            break;
		case 3:
			delete mProjectileTemplate;
			mProjectileTemplate = new CannonClusterProjectile(false);
			mProjectileTemplate->SetOwner(this);
			mAttackSpeed *= 0.9f;

			mCustomUpgradeCost = 3000;
            break;
		case 4:
			delete mProjectileTemplate;
			mProjectileTemplate = new CannonClusterProjectile(true);
			mProjectileTemplate->SetOwner(this);
			mAttackSpeed *= 0.8f;
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
