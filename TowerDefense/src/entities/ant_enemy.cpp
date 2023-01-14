#include "ant_enemy.h"
#include "globals.hpp"

AntEnemy::AntEnemy(Point2 pos)
	: Enemy(pos, 5, 1, 15)
{
	mTexture = Globals::gResources->GetTexture("ant_alpha");
	SetScale(.1f);
}

AntEnemy::~AntEnemy()
{

}
