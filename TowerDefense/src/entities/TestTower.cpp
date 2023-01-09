#include "TestTower.h"

#include "Globals.h"

TestTower::TestTower(Texture* texture)
	: Tower(new Projectile(TEST_TOWER_DAMAGE, TEST_TOWER_PIERCE, 3))
{
	setWidth(1);
	setHeight(1);
	setRange(5);
	setTexture(texture);
}

TestTower::TestTower(Point2 pixelPosition)
	: Tower(pixelPosition, TEST_TOWER_RANGE, TEST_TOWER_ATTACK_SPEED, new Projectile(TEST_TOWER_DAMAGE, TEST_TOWER_PIERCE, 3))
{
}

void TestTower::draw()
{
	Globals::gDrawList->AddImage(getTexture()->id, getPixelPosition(),
		ImVec2(getPixelPosition().x + getWidth(), getPixelPosition().y + getHeight()));
}

void TestTower::shoot(const Projectile& projTemplate)
{
}
