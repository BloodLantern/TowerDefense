#pragma once

#include "Entity.h"
#include "Projectile.h"
#include "Enemy.h"

#include <stdint.h>

class Player;

class Tower : public Entity
{
public:
	Tower(const Tower& other);
	Tower(Projectile* projectileTemplate);
	Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate);
	virtual ~Tower() {};

	virtual void Shoot(const Projectile& projTemplate);
	void Draw() override;

    float_t GetRange() const { return mRange; }
	uint8_t GetWidth() const { return mWidth; }
	uint8_t GetHeight() const { return mHeight; }
	Player* GetOwner() const { return mOwner; }
	void SetOwner(Player* newOwner) { mOwner = newOwner; }

protected:
	void SetRange(float range) { mRange = range; }
	void SetWidth(uint8_t newWidth) { mWidth = newWidth; }
	void SetHeight(uint8_t newHeight) { mHeight = newHeight; }

private:
	float_t mRange = 3.f;
	// Number of attacks per second
	float_t mAttackSpeed = 1.f;

	Projectile& mProjectileTemplate;
	Enemy* mTarget;

	uint8_t mGenericUpgradeLevel = 1;
	uint8_t mCustomUpgradeLevel = 1;

	uint8_t mWidth = 1;
	uint8_t mHeight = 1;

	uint32_t mDamageDealt = 0;
	uint32_t mKillCount = 0;
	uint32_t mMoneyGenerated = 0;

	Player* mOwner;
};
