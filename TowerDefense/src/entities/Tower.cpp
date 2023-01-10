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
	mOwner = nullptr;
	mTarget = nullptr;
}

Tower::Tower(Projectile* projectileTemplate)
	: Entity(), mProjectileTemplate(*projectileTemplate)
{
	mOwner = nullptr;
	mTarget = nullptr;
}

Tower::Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate)
	: Entity(pixelPosition), mRange(range), mAttackSpeed(attackSpeed), mProjectileTemplate(*projectileTemplate)
{
	mCustomUpgradeLevel = 0;
	mGenericUpgradeLevel = 0;

	mOwner = nullptr;
	mTarget = nullptr;
}

void Tower::Shoot(const Projectile& projTemplate)
{
	
}

void Tower::OnUpdate()
{
	ImVec2 mouseClickedPos = Globals::gIO->MouseClickedPos[ImGuiMouseButton_Left];
	// If clicked on the tower
	if ((int)(mouseClickedPos.x - Globals::gGridX) / GRID_SQUARE_SIZE == GetTilePosition().x
		&& (int)(mouseClickedPos.y - Globals::gGridY) / GRID_SQUARE_SIZE == GetTilePosition().y)
	{
		std::cout << "Mouse clicked" << std::endl;
	}
}

void Tower::OnRender()
{
	//ImVec2 pos(GetPixelPosition().x + Globals::gGridX + GRID_SQUARE_SIZE / 2, GetPixelPosition().y + Globals::gGridY + GRID_SQUARE_SIZE / 2);
	//ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, ImVec2(GetScale(), GetScale()), GetRotation());
	ImVec2 topLeft = ImVec2(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y);
	ImVec2 bottomRight = ImVec2(topLeft.x + GetWidth() * GRID_SQUARE_SIZE, topLeft.y + GetHeight() * GRID_SQUARE_SIZE);
	Globals::gDrawList->AddImage(GetTexture()->id, topLeft, bottomRight);
}
