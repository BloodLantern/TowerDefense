#include "ant_enemy.hpp"
#include "globals.hpp"

AntEnemy::AntEnemy(Point2 pos)
	: Enemy(pos, 5, 1, 15)
{
	mTexture = Globals::gResources->GetTexture("enemies\\ant_alpha");
	SetScale(.1f);
}

AntEnemy::~AntEnemy()
{

}
