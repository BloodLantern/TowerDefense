#include "mite_enemy.hpp"
#include "enemies_data.hpp"
#include "globals.hpp"

MiteEnemy::MiteEnemy(Point2 pos)
	: Enemy(pos, sEnemyInfo[EnemyId::MITE].health, sEnemyInfo[EnemyId::MITE].damage, sEnemyInfo[EnemyId::MITE].moneyDrop)
{
	mTexture = Globals::gResources->GetTexture("enemies\\mite");

	mSpeed = .9f;
	SetScale(.1f);
}

MiteEnemy::~MiteEnemy()
{

}
