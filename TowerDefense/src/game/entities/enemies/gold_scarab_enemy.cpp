#include "gold_scarab_enemy.hpp"
#include "enemies_data.hpp"
#include "globals.hpp"

GoldScarabEnemy::GoldScarabEnemy(Point2 position)
	: Enemy(position, sEnemyInfo[EnemyId::GOLD_SCARAB].health, sEnemyInfo[EnemyId::GOLD_SCARAB].damage, sEnemyInfo[EnemyId::GOLD_SCARAB].moneyDrop)
{
	mTexture = Globals::gResources->GetTexture("enemies\\gold_scarab");
	mSpeed = 3.f;
	SetScale(.1f);
}

GoldScarabEnemy::~GoldScarabEnemy()
{

}
