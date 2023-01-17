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
	bool CheckInHiveRange();
	bool CheckAtHive();
	void FindTarget();

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

	BeehiveTower* mHive;

	float_t mHookedTimer;
	float_t mIdleVelocityTimer;

	BeePose mPose;
};

