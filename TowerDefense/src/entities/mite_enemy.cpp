#include "mite_enemy.h"
#include "globals.hpp"

MiteEnemy::MiteEnemy(Point2 pos)
	: Enemy(pos, 15, 2, 20)
{
	mTexture = Globals::gResources->GetTexture("enemies\\mite");
	SetScale(.1f);
}

MiteEnemy::~MiteEnemy()
{

}
