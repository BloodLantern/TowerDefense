#include "mite_enemy.hpp"
#include "globals.hpp"

MiteEnemy::MiteEnemy(Point2 pos)
	: Enemy(pos, 15, 2, 20)
{
	mTexture = Globals::gResources->GetTexture("enemies\\mite");

	mSpeed = .9f;
	SetScale(.1f);
}

MiteEnemy::~MiteEnemy()
{

}
