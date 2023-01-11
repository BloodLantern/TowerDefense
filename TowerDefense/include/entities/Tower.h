#pragma once

#include "Entity.h"
#include "Projectile.h"
#include "Enemy.h"

#include <stdint.h>
#include <string>

class Player;

#define TOWER_RANGE_COLOR_AVAILABLE IM_COL32(0xB0, 0xB0, 0xB0, 0x70)

class Tower : public Entity
{
public:
	Tower(const Tower& other);
	Tower(Projectile* projectileTemplate);
	Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate);
	virtual ~Tower() {};

	virtual void Shoot(const Projectile& projTemplate);
	void DrawRange(ImU32 color = TOWER_RANGE_COLOR_AVAILABLE) const;

	virtual void OnRender() override;
	virtual void OnUpdate() override;

    std::string GetName() const { return mName; }
    float_t GetRange() const { return mRange; }
	uint8_t GetWidth() const { return mWidth; }
	uint8_t GetHeight() const { return mHeight; }
	Player* GetOwner() const { return mOwner; }
	void SetOwner(Player* newOwner) { mOwner = newOwner; }

protected:
	void SetName(std::string newName) { mName = newName; }
	void SetRange(float_t range) { mRange = range; }
	void SetWidth(uint8_t newWidth) { mWidth = newWidth; }
	void SetHeight(uint8_t newHeight) { mHeight = newHeight; }

private:
    std::string mName;

	float_t mRange = 3.f;
	// Number of attacks per second
	float_t mAttackSpeed = 1.f;

	Projectile& mProjectileTemplate;
	Enemy* mTarget;

	uint8_t mGenericUpgradeLevel = 1;
	uint8_t mCustomUpgradeLevel = 1;

	uint8_t mWidth = 1, mHeight = 1;

	uint32_t mDamageDealt = 0;
	uint32_t mKillCount = 0;
	uint32_t mMoneyGenerated = 0;

	Player* mOwner;

	bool mSelected;
};
