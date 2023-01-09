#include "Tower.h"

#include "Globals.h"

Tower::Tower(const Tower& other)
	: Entity(other),
	mRange(other.mRange),
	mAttackSpeed(other.mAttackSpeed),
	mProjectileTemplate(other.mProjectileTemplate),
	mWidth(other.mWidth),
	mHeight(other.mHeight)
{
}

Tower::Tower(Projectile* projectileTemplate)
	: Entity(), mProjectileTemplate(*projectileTemplate)
{

}

Tower::Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate)
	: Entity(pixelPosition), mRange(range), mAttackSpeed(attackSpeed), mProjectileTemplate(*projectileTemplate)
{
	mCustomUpgradeLevel = 0;
	mGenericUpgradeLevel = 0;

	mTarget = nullptr;
}

void Tower::Shoot(const Projectile& projTemplate)
{

}

void Tower::Draw()
{
	ImVec2 topLeft = ImVec2(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y);
	ImVec2 bottomRight = ImVec2(topLeft.x + GetWidth() * GRID_SQUARE_SIZE, topLeft.y + GetHeight() * GRID_SQUARE_SIZE);
	Globals::gDrawList->AddImage(GetTexture()->id, topLeft, bottomRight);
}
