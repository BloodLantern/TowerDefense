#include "firefly_enemy.hpp"
#include "globals.hpp"

FireflyEnemy::FireflyEnemy(Point2 position)
	: Enemy(position, 250, 1, 75), mTarget(*this)
{
	mTexture = Globals::gResources->GetTexture("enemies\\firefly");
	mScale = .1f;
	mHealTimer = 1.f;

	mHealParticlesEmitter = new ParticleEmitter(GetPixelPosition(), *Globals::gResources->GetTexture("particles\\heart_heal"));
	mHealParticlesEmitter->ConfigureEmitter(5, .3f, 0.05f, 0.1f, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), Vector2(0, -10), 10.f, 1.f, false);
}

void FireflyEnemy::OnUpdate()
{
	Enemy::OnUpdate();

	FindHealTarget();
	HealTarget();
}

void FireflyEnemy::FindHealTarget()
{

}

void FireflyEnemy::HealTarget()
{
	uint32_t health = mTarget.GetHealth();
	uint32_t spawnHealth = mTarget.GetSpawnHealth();

	if (health == spawnHealth)
		return;

	mHealParticlesEmitter->SetPosition(mTarget.GetPixelPosition());

	mHealTimer -= Globals::gGame->GetPlayingSpeedDeltaTime();
	if (mHealTimer < 0)
	{
		mHealTimer = 1.f;
		mHealParticlesEmitter->Start();

		if (mTarget.Heal(10))
		{
			mTarget = *this;
			mHealParticlesEmitter->Stop();
		}
	}
}
