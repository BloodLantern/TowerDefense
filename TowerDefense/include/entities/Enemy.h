#pragma once

#include "Entity.h"
#include <stdint.h>
#include <Vector2.h>


class Enemy : public Entity
{
public:
	Enemy(Point2 pixelPosition, uint32_t health, uint8_t damage, uint16_t moneyDrop);

	virtual void OnUpdate() override;
	virtual void OnRender() override;

	virtual void StickToPath();

private:
	Vector2 mVelocity;
	uint32_t mHealth;
	uint8_t mDamage;

	uint16_t mMoneyDrop;
	bool mSlowed;

	uint16_t mCurrentPathIndex;
};
