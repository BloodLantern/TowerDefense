#include "minigun_tower.hpp"
#include "globals.hpp"

#define CUSTOM_UPGRADE_LEVEL_MAX 3

#define MINIGUN_LENGTH 280
#define FLAME_SCALE Vector2(0.03f)

static const char* const sCustomUpgradeTooltips[CUSTOM_UPGRADE_LEVEL_MAX] = {
	"Longer cannon: can shoot further.",
	"Stronger bullets: deals slightly more damage.",
	"Ludicrous firing rate: gets 3x more base attack speed!"
};

enum MinigunCustomUpgrades
{
    LONGER_CANNON = 1,
    STRONGER_BULLETS,
    LUDICROUS_FIRING_RATE
};

MinigunTower::MinigunTower(Texture* texture)
	: Tower(8.f, 4.f, "Minigun", 100, texture),
	mHandleTexture(Globals::gResources->GetTexture("towers\\handle")),
	mMinigunTexture(Globals::gResources->GetTexture("towers\\minigun")),
	mFlameTexture(Globals::gResources->GetTexture("particles\\flame"))
{
	mStartDamage = 2;
	mDamage = 2;

    mScale = 0.1f;

	mCustomUpgradeCost = 300;
	mCustomUpgradeLevelMax = CUSTOM_UPGRADE_LEVEL_MAX;
}

void MinigunTower::OnUpdate()
{
    Tower::OnUpdate();

    if (mFlameAlpha > 0x40)
        mFlameAlpha -= 0x0B;
    else
        mFlameAlpha = 0;
}

Tower* MinigunTower::Clone() const
{
	MinigunTower* result = new MinigunTower(mTexture);
	result->mWidth = mWidth;
	result->mHeight = mHeight;
	return result;
}

void MinigunTower::Shoot()
{
	uint32_t damageDealt = 0;
    // If the enemy died, update tower kill stat
    if (!mTarget->toDelete && mTarget->DealDamage(mDamage, damageDealt))
    {
        IncreaseKillCount(1);
        IncreaseMoneyGenerated(mTarget->GetMoneyDrop());
    }
    IncreaseDamageDealt(damageDealt);

    mFlameAlpha = 0xFF;
}

void MinigunTower::OnCustomUpgrade()
{
	switch (customUpgradeLevel)
	{
		case LONGER_CANNON:
			mStartRange += 1.5f;
            UpdateRange();

			mCustomUpgradeCost = 500;
            break;
		case STRONGER_BULLETS:
		    mDamage++;

			mCustomUpgradeCost = 1500;
            break;
		case LUDICROUS_FIRING_RATE:
			mStartAttackSpeed *= 3;
            UpdateAttackSpeed();
            break;
	}
}

const char *const MinigunTower::GetCustomUpgradeTooltip(uint8_t level) const
{
    return sCustomUpgradeTooltips[level];
}

void MinigunTower::DrawTowerBarTexture()
{
	const ImVec2 pos(Globals::gGridX + GetPixelPosition().x + mWidth * GRID_SQUARE_SIZE / 2, Globals::gGridY + GetPixelPosition().y + mHeight * GRID_SQUARE_SIZE / 2);
	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *mTexture, pos, mScale, mRotation);
}

void MinigunTower::Draw(const ImVec2 &topLeft, const ImVec2 &bottomRight) const
{
	Globals::gDrawList->AddImage(mHandleTexture->id, topLeft, bottomRight);

	ImVec2 pos = topLeft;
	pos.x += mWidth * GRID_SQUARE_SIZE / 2;
	pos.y += mHeight * GRID_SQUARE_SIZE / 2 - 6;
	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *mMinigunTexture, pos, mScale, mRotation);

    Point2 flamePos = pos;
    flamePos.x += MINIGUN_LENGTH * mScale.x;
    flamePos.y -= 2;
    flamePos = flamePos.Rotate(pos, mRotation);

    Vector2 flameScale = FLAME_SCALE;
    if (mScale.x < 0)
        flameScale.x *= -1;

	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *mFlameTexture, flamePos, flameScale, mRotation, IM_COL32(0xFF, 0xFF, 0xFF, mFlameAlpha));
}
