#pragma once

#include "tower.hpp"

#define TEST_TOWER_RANGE 5.f
#define TEST_TOWER_PROJECTILE_SPEED 15
#define TEST_TOWER_DAMAGE 1
#define TEST_TOWER_ATTACK_SPEED 0.5f
#define TEST_TOWER_PIERCE 1
#define TEST_TOWER_LIFETIME 0.5f

class TestTower : public Tower
{
public:
	TestTower(const TestTower& other);
	TestTower(Texture* texture);
	TestTower(Point2 pixelPosition);

private:

};
