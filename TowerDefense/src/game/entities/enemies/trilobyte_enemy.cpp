#include "trilobyte_enemy.hpp"
#include "enemies_data.hpp"
#include "globals.hpp"

TrilobiteEnemy::TrilobiteEnemy(Point2 position)
	: Enemy(position, sEnemyInfo[EnemyId::TRILOBITE].health, sEnemyInfo[EnemyId::TRILOBITE].damage, sEnemyInfo[EnemyId::TRILOBITE].moneyDrop)
{
	mTexture = Globals::gResources->GetTexture("enemies\\trilobite");

	mSpeed = .5f;
	SetScale(.1f);
}
