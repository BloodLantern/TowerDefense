#include "ladybug_enemy.hpp"

#include "globals.hpp"

LadybugEnemy::LadybugEnemy(Point2 pos)
	: Enemy(pos, 150, 3, 40)
{
	mTexture = Globals::gResources->GetTexture("enemies\\ladybug");
	SetScale(.1f);
}

LadybugEnemy::~LadybugEnemy()
{

}

void LadybugEnemy::OnUpdate()
{
	Enemy::OnUpdate();

	mSpeed = 3.f - 2 * (mHealth / (float_t)mSpawnHealth);
}
