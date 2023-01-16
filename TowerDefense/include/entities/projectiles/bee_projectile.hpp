#pragma once

#include "projectile.hpp"
#include "beehive_tower.hpp"

class BeeProjectile : public Projectile
{
public:
	BeeProjectile(BeehiveTower* hive);
	~BeeProjectile();

	void OnUpdate() override;
	void OnRender() override;
	void HandleEnemyCollision() override;

	Projectile* Clone() const override;

private:
	BeehiveTower* mHive;

	float_t mHookedTimer;
};

