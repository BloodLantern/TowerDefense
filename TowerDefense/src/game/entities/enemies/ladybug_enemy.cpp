#include "ladybug_enemy.hpp"
#include "enemies_data.hpp"
#include "globals.hpp"

LadybugEnemy::LadybugEnemy(Point2 pos)
	: Enemy(pos, sEnemyInfo[EnemyId::LADYBUG].health, sEnemyInfo[EnemyId::LADYBUG].damage, sEnemyInfo[EnemyId::LADYBUG].moneyDrop)
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
