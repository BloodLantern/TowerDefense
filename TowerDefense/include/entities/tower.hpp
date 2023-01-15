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

#define TOWER_UPGRADE_GENERIC_LEVEL_MAX 10
#define TOWER_UPGRADE_GENERIC_COST_BASE 20
#define TOWER_UPGRADE_GENERIC_COST_MULTIPLIER 1.3f
#define TOWER_UPGRADE_GENERIC_RANGE_MULTIPLIER 0.2f
#define TOWER_UPGRADE_GENERIC_DAMAGE_MULTIPLIER 1.1f
#define TOWER_UPGRADE_GENERIC_ATTACK_SPEED_MULTIPLIER 0.15f

class Tower : public Entity
{
	enum GenericUpgradeType : uint8_t
	{
		DAMAGE = 0,
		ATTACK_SPEED = 1,
		RANGE = 2
	};

public:
	Tower(const Tower& other);
	Tower(Projectile* projectileTemplate);
	Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate);
	//virtual ~Tower() { delete mProjectileTemplate; };

	virtual void Shoot(Projectile* projTemplate);
	void DrawRange(ImU32 color = TOWER_RANGE_COLOR_AVAILABLE) const;

	virtual void OnRender() override;
	virtual void OnUpdate() override;

	std::string GetName() const { return mName; }
	uint32_t GetCost() const { return mCost; }
	uint32_t GetDamage() const { return mDamage + mGenericUpgradeLevels[GenericUpgradeType::DAMAGE] * TOWER_UPGRADE_GENERIC_DAMAGE_MULTIPLIER; }
	float_t GetAttackSpeed() const { return mAttackSpeed + mGenericUpgradeLevels[GenericUpgradeType::ATTACK_SPEED] * TOWER_UPGRADE_GENERIC_ATTACK_SPEED_MULTIPLIER; }
	float_t GetRange() const { return mRange + mGenericUpgradeLevels[GenericUpgradeType::RANGE] * TOWER_UPGRADE_GENERIC_RANGE_MULTIPLIER; }
	uint8_t GetWidth() const { return mWidth; }
	uint8_t GetHeight() const { return mHeight; }
	Player* GetOwner() const { return mOwner; }
	void SetOwner(Player* newOwner) { mOwner = newOwner; }
	void IncreaseDamageDealt(uint32_t damage) { mDamageDealt += damage; }
	void IncreaseKillCount(uint32_t kills) { mKillCount += kills; }
	void IncreaseMoneyGenerated(uint32_t money) { mMoneyGenerated += money; }
	uint32_t GetSellingPrice() const { return mMoneyInvested * mSellingFactor; }
	void SetTimeSinceLastAttack(double_t newTimeSinceLastAttack) { mTimeSinceLastAttack = newTimeSinceLastAttack; }

protected:
	void SetName(std::string newName) { mName = newName; }
	void SetCost(uint32_t newCost) { mCost = newCost; }
	void SetDamage(uint32_t newDamage) { mDamage = newDamage; }
	void SetAttackSpeed(float_t newAttackSpeed) { mAttackSpeed = newAttackSpeed; }
	void SetRange(float_t range) { mRange = range; }
	void SetWidth(uint8_t newWidth) { mWidth = newWidth; }
	void SetHeight(uint8_t newHeight) { mHeight = newHeight; }

private:
	std::string mName;
	
	uint32_t mCost = 100;
	uint32_t mMoneyInvested = mCost;
	float_t mSellingFactor = 0.7f;

	float_t mRange = 3.f;
	// Number of attacks per second
	float_t mAttackSpeed = 1.f;
	double_t mTimeSinceLastAttack = DBL_MAX;
	uint32_t mDamage = 1;

	Projectile* mProjectileTemplate;
	Enemy* mTarget;

	uint16_t mGenericUpgradeLevels[3] = { 0, 0, 0 };
	uint8_t mCustomUpgradeLevel = 0;

	uint8_t mWidth = 1, mHeight = 1;

	uint32_t mDamageDealt = 0;
	uint32_t mKillCount = 0;
	uint32_t mMoneyGenerated = 0;

	Player* mOwner;

	bool mSelected;

	void IncrementGenericUpgrade(GenericUpgradeType upgrade);
	uint32_t GetGenericUpgradeCost(GenericUpgradeType upgrade) { return TOWER_UPGRADE_GENERIC_COST_BASE * std::pow(TOWER_UPGRADE_GENERIC_COST_MULTIPLIER, mGenericUpgradeLevels[upgrade]); };

	void HandleSelection();
	void HandleShoot();
	void HandlePanel(const ImVec2& topLeft, const ImVec2& bottomRight);
	void DrawUpgrades(const ImVec2& panelPosition, ImDrawList* dl);
	void DrawStats();
	void DisplayTowerUpgrade(GenericUpgradeType upgrade);
};
