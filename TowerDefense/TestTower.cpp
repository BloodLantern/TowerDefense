#include "TestTower.h"

TestTower::TestTower(Point2 pixelPosition)
	: Tower(pixelPosition, TEST_TOWER_RANGE, TEST_TOWER_ATTACK_SPEED, new Projectile(TEST_TOWER_DAMAGE, TEST_TOWER_PIERCE, 3))
{
}

void TestTower::draw()
{
}

void TestTower::shoot(const Projectile& projTemplate)
{
}
