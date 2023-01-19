#include "ant_enemy.hpp"
#include "globals.hpp"
#include "enemies_data.hpp"

AntEnemy::AntEnemy(Point2 pos)
	: Enemy(pos, sEnemyInfo[EnemyId::ANT].health, sEnemyInfo[EnemyId::ANT].damage, sEnemyInfo[EnemyId::ANT].moneyDrop)
{
	mTexture = Globals::gResources->GetTexture("enemies\\ant_alpha");
	SetScale(.1f);
}

AntEnemy::~AntEnemy()
{

}
