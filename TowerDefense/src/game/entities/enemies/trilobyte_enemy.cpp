#include "trilobyte_enemy.hpp"
#include "globals.hpp"

TrilobiteEnemy::TrilobiteEnemy(Point2 position)
	: Enemy(position, 1000, 10, 100)
{
	mTexture = Globals::gResources->GetTexture("enemies\\trilobite");

	mSpeed = .5f;
	SetScale(.1f);
}
