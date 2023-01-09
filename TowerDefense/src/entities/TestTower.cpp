#include "TestTower.h"

#include "Globals.h"

TestTower::TestTower(const TestTower& other)
	: Tower(other)
{
}

TestTower::TestTower(Texture* texture)
	: Tower(new Projectile(TEST_TOWER_DAMAGE, TEST_TOWER_PIERCE, 3))
{
	SetWidth(1);
	SetHeight(1);
	SetRange(5);
	SetTexture(texture);
}

TestTower::TestTower(Point2 pixelPosition)
	: Tower(pixelPosition, TEST_TOWER_RANGE, TEST_TOWER_ATTACK_SPEED, new Projectile(TEST_TOWER_DAMAGE, TEST_TOWER_PIERCE, 3))
{
}

void TestTower::Shoot(const Projectile& projTemplate)
{
}
