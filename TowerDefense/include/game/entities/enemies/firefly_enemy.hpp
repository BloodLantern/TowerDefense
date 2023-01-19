#pragma once

#include "enemy.hpp"
#include "particle_emitter.hpp"

class FireflyEnemy : public Enemy
{
public:
	FireflyEnemy(Point2 position);
	~FireflyEnemy();

	void OnUpdate() override;

private:
	Enemy* mTarget;

	ParticleEmitter* mHealParticlesEmitter;
	float_t mHealTimer;

	void FindHealTarget();
	void HealTarget();
	bool CheckInHealingRange(Enemy& e);
};

