#pragma once

#include "projectile.hpp"

class BeenestTower;

class BeeProjectile : public Projectile
{
public:
	BeeProjectile(BeenestTower* hive);
	~BeeProjectile();

	void UpdateForDarts();

	void OnUpdate() override;
	void OnRender() override;
	void HandleEnemyCollision() override;

	Projectile* Clone() const override;

	bool slowEnemies;

private:
	bool CheckInHiveRange();
	bool CheckAtHive();
	void FindTarget();

	void MakeFaceDirection();

	void HandleIdleFloating();
	void SetGoingBackToHive();
	void HandleGoingToTarget();

	enum BeePose
	{
		BEE_POSE_IDLE,
		BEE_POSE_BACK_TO_HIVE,
		BEE_POSE_GOING_TO_TARGET,
		BEE_POSE_ON_TARGET,
	};

	BeenestTower* mHive;

	float_t mHookedTimer;
	float_t mIdleVelocityTimer;
	float_t mDartTimer;

	BeePose mPose;
};

