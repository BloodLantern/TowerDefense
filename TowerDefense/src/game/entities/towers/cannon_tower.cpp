#include "cannon_tower.hpp"
#include "cannon_ball_projectile.hpp"
#include "globals.hpp"

#define CUSTOM_UPGRADE_LEVEL_MAX 4

static const char* const sCustomUpgradeTooltips[CUSTOM_UPGRADE_LEVEL_MAX] = {
	"Shoots faster bombs: never miss again!",
	"Increases damage: now it is also useful in the mid-game.",
	"Cluster bombs: on explosion, the bomb now unleashes 8 other bombs around!",
	"Recursive cluster: cluster bombs now also release more cluster bombs!"
};

CannonTower::CannonTower(Texture* texture)
	: Tower(new CannonBallProjectile(), 0.4f, 5.5f, "Cannon", 25, texture),
	mHandleTexture(Globals::gResources->GetTexture("towers\\cannon_handle")),
	mCannonTexture(Globals::gResources->GetTexture("towers\\cannon"))
{
	mScale = 0.07f;

	mCustomUpgradeCost = 5;
	mCustomUpgradeLevelMax = CUSTOM_UPGRADE_LEVEL_MAX;
}

void CannonTower::OnCustomUpgrade()
{
	switch (mCustomUpgradeLevel)
	{
		case 1:
            break;
		case 2:
            break;
		case 3:
            break;
		case 4:
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
