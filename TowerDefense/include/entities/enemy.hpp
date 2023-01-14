#pragma once

#include "entity.hpp"
#include <stdint.h>
#include <Vector2.h>


class Enemy : public Entity
{
public:
	Enemy(Point2 pixelPosition, uint32_t health, uint8_t damage, uint32_t moneyDrop);

	virtual void OnUpdate() override;
	virtual void OnRender() override;

	virtual void StickToPath();

	// Returns whether the enemy died
	bool DealDamage(uint32_t damage, uint32_t& damageDealt);

	uint32_t GetMoneyDrop() const { return mMoneyDrop; }

private:
	Vector2 mVelocity;
	float mSpeed;

	uint32_t mHealth;
	uint32_t mSpawnHealth;
	uint8_t mDamage;

	uint32_t mMoneyDrop;
	bool mSlowed;

	uint16_t mCurrentPathIndex;
};
