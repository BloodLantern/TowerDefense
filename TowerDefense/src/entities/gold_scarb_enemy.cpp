#include "gold_scarb_enemy.hpp"

#include "globals.hpp"

GoldScarabEnemy::GoldScarabEnemy(Point2 pos)
	: Enemy(pos, 7, 2, 30)
{
	mTexture = Globals::gResources->GetTexture("enemies\\gold_scarab");
	mSpeed = 3.f;
	SetScale(.1f);
}

GoldScarabEnemy::~GoldScarabEnemy()
{

}
