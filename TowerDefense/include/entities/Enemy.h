#pragma once

#include "Entity.h"
#include <stdint.h>
#include <Vector2.h>


class Enemy : public Entity
{
public:
	Enemy(Point2 pixelPosition, uint32_t health, uint8_t damage, uint16_t moneyDrop);

	void draw() override;

private:
	Vector2 m_velocity;
	uint32_t m_health;
	uint8_t m_damage;

	uint16_t m_moneyDrop;
	bool m_slowed;
};
