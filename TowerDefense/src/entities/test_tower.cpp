#include "test_tower.hpp"
#include "globals.hpp"

TestTower::TestTower(const TestTower& other)
	: Tower(other)
{
}

TestTower::TestTower(Texture* texture)
	: Tower(new Projectile(TEST_TOWER_PROJECTILE_SPEED, TEST_TOWER_DAMAGE, TEST_TOWER_PIERCE, TEST_TOWER_LIFETIME))
{
	SetName("TestTower");

	SetDamage(TEST_TOWER_DAMAGE);
	SetAttackSpeed(TEST_TOWER_ATTACK_SPEED);
	SetRange(TEST_TOWER_RANGE);

	SetCost(100);
	SetWidth(1);
	SetHeight(1);
	SetTexture(texture);
}

TestTower::TestTower(Point2 pixelPosition)
	: Tower(pixelPosition, TEST_TOWER_RANGE, TEST_TOWER_ATTACK_SPEED, new Projectile(TEST_TOWER_PROJECTILE_SPEED, TEST_TOWER_DAMAGE, TEST_TOWER_PIERCE, 3))
{
}
