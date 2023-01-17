#include "test_tower.hpp"
#include "globals.hpp"

TestTower::TestTower(Texture* texture)
	: Tower(new Projectile(12.f, 1, 0, 0.5f), 0.5f, 5.f, "TestTower", 100, texture)
{
}

Tower *TestTower::Clone() const
{
	TestTower* result = new TestTower(mTexture);
	result->mWidth = mWidth;
	result->mHeight = mHeight;
	return result;
}
