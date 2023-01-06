#include "Enemy.h"

Enemy::Enemy(Point2 pixelPosition, uint32_t health, uint8_t damage, uint16_t moneyDrop)
	: Entity(pixelPosition), m_health(health), m_damage(damage), m_moneyDrop(moneyDrop)
{
}

void Enemy::draw()
{
}
