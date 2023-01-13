#pragma once

#include "tower.hpp"

#define TEST_TOWER_RANGE 5.f
#define TEST_TOWER_PROJECTILE_SPEED 5
#define TEST_TOWER_DAMAGE 2
#define TEST_TOWER_ATTACK_SPEED 5.f
#define TEST_TOWER_PIERCE 2

class TestTower : public Tower
{
public:
	TestTower(const TestTower& other);
	TestTower(Texture* texture);
	TestTower(Point2 pixelPosition);

	void Shoot(Projectile* projTemplate) override;

	void OnRender() override;
	void OnUpdate() override;

private:

};
