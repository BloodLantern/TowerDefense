#include "firefly_enemy.hpp"
#include "globals.hpp"

#define HEAL_RANGE 5.f

FireflyEnemy::FireflyEnemy(Point2 position)
	: Enemy(position, 250, 1, 75), mTarget(this)
{
	mTexture = Globals::gResources->GetTexture("enemies\\firefly");
	mScale = .1f;
	mHealTimer = 1.f;

	mHealParticlesEmitter = new ParticleEmitter(GetPixelPosition(), *Globals::gResources->GetTexture("particles\\heart_heal"));
	mHealParticlesEmitter->ConfigureEmitter(5, 1.f, .1f, 0.1f, IM_COL32(0xFF, 0xFF, 0xFF, 0xFF), Vector2(0, -10), 100.f, 10.f, false);
}

FireflyEnemy::~FireflyEnemy()
{
	delete mHealParticlesEmitter;
}

void FireflyEnemy::OnUpdate()
{
	Enemy::OnUpdate();

	FindHealTarget();
	HealTarget();

	mHealParticlesEmitter->Update();
}

void FireflyEnemy::FindHealTarget()
{
	if (mTarget != this)
		return;

	for (std::vector<Enemy*>::iterator _e = Globals::gGame->enemies.begin(); _e != Globals::gGame->enemies.end(); _e++)
	{
		Enemy* e = *_e;
		
		// Already full health
		if (e->GetHealth() == e->GetSpawnHealth())
			continue;

		if (CheckInHealingRange(*e))
			mTarget = e;
	}
}

void FireflyEnemy::HealTarget()
{
	uint32_t health = mTarget->GetHealth();
	uint32_t spawnHealth = mTarget->GetSpawnHealth();

	if (health == spawnHealth || !CheckInHealingRange(*mTarget))
	{
		mTarget = this;
		mHealParticlesEmitter->Stop();
		return;
	}

	mHealParticlesEmitter->SetPosition(mTarget->GetPixelPosition());

	mHealTimer -= Globals::gGame->GetPlayingSpeedDeltaTime();
	if (mHealTimer < 0)
	{
		mHealTimer = 1.f;
		mHealParticlesEmitter->Start();

		if (mTarget->Heal(10))
		{
			mTarget = this;
			mHealParticlesEmitter->Stop();
		}
	}
}

bool FireflyEnemy::CheckInHealingRange(Enemy& e)
{
	float_t range = HEAL_RANGE * GRID_SQUARE_SIZE;
	range *= range;

	Vector2 dist = Vector2(GetPixelPosition(), e.GetPixelPosition());
	return dist.GetSquaredNorm() < range;
}
