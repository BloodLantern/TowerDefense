#pragma once

#include "Entity.h"
#include <stdint.h>
#include <Vector2.h>


class Enemy : public Entity
{
public:

private:
	Vector2 m_velocity;
	uint32_t m_health;
	uint8_t m_damage;

	uint16_t m_moneyDrop;
};
