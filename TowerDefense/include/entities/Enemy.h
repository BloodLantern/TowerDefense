#pragma once

#include "Entity.h"
#include <stdint.h>
#include <Vector2.h>


class Enemy : public Entity
{
public:
	Enemy(Point2 pixelPosition, uint32_t health, uint8_t damage, uint16_t moneyDrop);

	void Draw() override;
	virtual void StickToPath();

private:
	Vector2 mVelocity;
	uint32_t mHealth;
	uint8_t mDamage;

	uint16_t mMoneyDrop;
	bool mSlowed;

	uint8_t mGridDestinationX;
	uint8_t mGridDestinationY;
	uint16_t mCurrentPathIndex;
};
