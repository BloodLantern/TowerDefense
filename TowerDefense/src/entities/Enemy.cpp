#include "Enemy.h"

Enemy::Enemy(Point2 pixelPosition, uint32_t health, uint8_t damage, uint16_t moneyDrop)
	: Entity(pixelPosition), mHealth(health), mDamage(damage), mMoneyDrop(moneyDrop), mSlowed(false)
{
}

void Enemy::Draw()
{
}

void Enemy::StickToPath()
{

}
