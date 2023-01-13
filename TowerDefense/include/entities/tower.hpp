#pragma once

#include "entity.hpp"
#include "projectile.hpp"
#include "enemy.hpp"

#include <stdint.h>
#include <string>

class Player;

#define TOWER_RANGE_COLOR_AVAILABLE IM_COL32(0xB0, 0xB0, 0xB0, 0x70)
#define TOWER_PANEL_TEXT_SIZE_BIG 40
#define TOWER_PANEL_TEXT_SIZE_MEDIUM 20

#define TOWER_UPGRADE_GENERIC_COST_MULTIPLIER 1.3f
#define TOWER_UPGRADE_GENERIC_RANGE_MULTIPLIER 0.25f
#define TOWER_UPGRADE_GENERIC_DAMAGE_MULTIPLIER 1.5f
#define TOWER_UPGRADE_GENERIC_ATTACK_SPEED_MULTIPLIER 1.05f

class Tower : public Entity
{
public:
	Tower(const Tower& other);
	Tower(Projectile* projectileTemplate);
	Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate);
	//virtual ~Tower() { delete mProjectileTemplate; };

	virtual void Shoot(const Projectile& projTemplate);
	void DrawRange(ImU32 color = TOWER_RANGE_COLOR_AVAILABLE) const;

	virtual void OnRender() override;
	virtual void OnUpdate() override;

	std::string GetName() const { return mName; }
	uint32_t GetCost() const { return mCost; }
	uint32_t GetDamage() const { return mDamage + mGenericUpgradeLevels[0] * TOWER_UPGRADE_GENERIC_DAMAGE_MULTIPLIER; }
	float_t GetAttackSpeed() const { return mAttackSpeed + mGenericUpgradeLevels[1] * TOWER_UPGRADE_GENERIC_ATTACK_SPEED_MULTIPLIER; }
	float_t GetRange() const { return mRange + mGenericUpgradeLevels[2] * TOWER_UPGRADE_GENERIC_RANGE_MULTIPLIER; }
	uint8_t GetWidth() const { return mWidth; }
	uint8_t GetHeight() const { return mHeight; }
	Player* GetOwner() const { return mOwner; }
	void SetOwner(Player* newOwner) { mOwner = newOwner; }

protected:
	void SetName(std::string newName) { mName = newName; }
	void SetCost(uint32_t newCost) { mCost = newCost; }
	void SetRange(float_t range) { mRange = range; }
	void SetWidth(uint8_t newWidth) { mWidth = newWidth; }
	void SetHeight(uint8_t newHeight) { mHeight = newHeight; }

private:
	std::string mName;
	
	uint32_t mCost = 100;

	float_t mRange = 3.f;
	// Number of attacks per second
	float_t mAttackSpeed = 1.f;
	uint32_t mDamage = 1;

	Projectile* mProjectileTemplate;
	Enemy* mTarget;

	uint8_t mGenericUpgradeLevels[3] = { 0, 0, 0 };
	uint8_t mCustomUpgradeLevel = 0;

	uint8_t mWidth = 1, mHeight = 1;

	uint32_t mDamageDealt = 0;
	uint32_t mKillCount = 0;
	uint32_t mMoneyGenerated = 0;

	Player* mOwner;

	bool mSelected;

	void IncrementGenericUpgrade(uint8_t upgrade);
	uint32_t GetGenericUpgradeCost(uint8_t upgrade) { return (mGenericUpgradeLevels[upgrade] + 1) * TOWER_UPGRADE_GENERIC_COST_MULTIPLIER; };

	void DisplayTowerUpgrade(uint8_t upgrade, std::string name);
};
