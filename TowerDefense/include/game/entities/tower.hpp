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

//#define TOWER_UPGRADE_GENERIC_DAMAGE

#define TOWER_UPGRADE_GENERIC_LEVEL_MAX 5
#define TOWER_UPGRADE_GENERIC_COST_BASE 40
#define TOWER_UPGRADE_GENERIC_COST_MULTIPLIER 1.6f
#define TOWER_UPGRADE_GENERIC_RANGE_MULTIPLIER 0.2f
#ifdef TOWER_UPGRADE_GENERIC_DAMAGE
#define TOWER_UPGRADE_GENERIC_DAMAGE_MULTIPLIER 1.1f
#endif
#define TOWER_UPGRADE_GENERIC_ATTACK_SPEED_MULTIPLIER 0.15f

enum UpgradeType : uint8_t
{
#ifdef TOWER_UPGRADE_GENERIC_DAMAGE
	DAMAGE = 0,
#endif
	ATTACK_SPEED = 1,
	RANGE = 2,
	CUSTOM = 3
};

class Tower : public Entity
{

public:
	Tower(Projectile* projectileTemplate, float_t attackSpeed, float_t range, std::string name, uint32_t cost, Texture* texture);
	Tower(float_t attackSpeed, float_t range, std::string name, uint32_t cost, Texture* texture);

	virtual Tower* Clone() const = 0;

	virtual void Shoot();
	void DrawRange(ImU32 color = TOWER_RANGE_COLOR_AVAILABLE) const;

	virtual void OnRender() override;
	virtual void DrawTowerBarTexture();
	virtual void OnUpdate() override;

	virtual void OnCustomUpgrade() = 0;

	static void InitUITextures();

	std::string GetName() const { return mName; }
	uint32_t GetCost() const { return mCost; }
	uint32_t GetDamage() const { return mDamage; }
	float_t GetAttackSpeed() const { return mAttackSpeed; }
	float_t GetRange() const { return mRange; }
	uint8_t GetWidth() const { return mWidth; }
	uint8_t GetHeight() const { return mHeight; }
	Player* GetOwner() const { return mOwner; }
	void SetOwner(Player* newOwner) { mOwner = newOwner; }
	void IncreaseDamageDealt(uint32_t damage) { mDamageDealt += damage; }
	void IncreaseKillCount(uint32_t kills) { mKillCount += kills; }
	void IncreaseMoneyGenerated(uint32_t money) { mMoneyGenerated += money; }
	uint32_t GetSellingPrice() const { return mMoneyInvested * mSellingFactor; }
	void SetTimeSinceLastAttack(double_t newTimeSinceLastAttack) { mTimeSinceLastAttack = newTimeSinceLastAttack; }
	Projectile* GetProjectileTemplate() const { return mProjectileTemplate; }

	// Needed for network stuff, bad
	uint16_t genericUpgradeLevels[3] = { 0, 0, 0 };
	void UpdateGeneric(UpgradeType upgrade);
	uint8_t customUpgradeLevel = 0;
protected:

	std::string mName;

	uint32_t mCost;

	uint32_t mStartDamage;
	float_t mStartAttackSpeed;
	float_t mStartRange;

	Projectile* mProjectileTemplate;
	Enemy* mTarget = nullptr;

	uint32_t mDamage;
	// Number of attacks per second
	float_t mAttackSpeed;
	float_t mRange;

	uint8_t mWidth = 1, mHeight = 1;

	double_t mTimeSinceLastAttack = DBL_MAX;

	bool mRotateTowardsEnemies = true;
	
	uint8_t mCustomUpgradeLevelMax = 0;
	uint32_t mCustomUpgradeCost = 1;

	void HandleSelection();
#ifdef TOWER_UPGRADE_GENERIC_DAMAGE
	virtual void UpdateDamage();
#endif
	virtual void UpdateAttackSpeed();
	virtual void UpdateRange();

private:
	uint32_t mMoneyInvested;
	float_t mSellingFactor = 0.7f;

	uint32_t mDamageDealt = 0;
	uint32_t mKillCount = 0;
	uint32_t mMoneyGenerated = 0;

	Player* mOwner = nullptr;

	bool mSelected = false;

	static Texture* mUpgradeIconTexture;
	static Texture* mKillIconTexture;
	static Texture* mDamageIconTexture;
	static Texture* mMoneyIconTexture;
	static Texture* mGenericUpgradeAttackDamageIconTexture;
	static Texture* mGenericUpgradeAtackSpeedIconTexture;
	static Texture* mGenericUpgradeRangeIconTexture;
	static Texture* mCustomUpgradeStarIcon;

	virtual const char* const GetCustomUpgradeTooltip(uint8_t level) const = 0;

	void IncrementGenericUpgrade(UpgradeType upgrade);
	uint32_t GetGenericUpgradeCost(UpgradeType upgrade) { return TOWER_UPGRADE_GENERIC_COST_BASE * std::pow(TOWER_UPGRADE_GENERIC_COST_MULTIPLIER, genericUpgradeLevels[upgrade]); };

	void HandleShoot();
	void RotateTowardsTarget();
	void HandlePanel(const ImVec2& topLeft, const ImVec2& bottomRight);
	// Returns whether the button was clicked
	bool DrawSellingButton();
	void DrawUpgrades(const ImVec2& panelPosition, ImDrawList* dl);
	// Returns whether the button was clicked
	bool DrawUpgradeButton(UpgradeType upgrade);
	void DrawStats();
	void DisplayGenericUpgrade(UpgradeType upgrade);
	void DisplayCustomUpgrade(const ImVec2& panelPosition, ImDrawList* dl);
	void AddTooltip(const char* text);
	void InitStats(uint32_t damage, float_t attackSpeed, float_t range, std::string name, uint32_t cost, Texture* texture);

	virtual void Draw(const ImVec2& topLeft, const ImVec2& bottomRight) const;
};
