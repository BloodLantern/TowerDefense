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

void Tower::DrawRange(ImU32 color) const
{
	Globals::gDrawList->AddCircleFilled(
		ImVec2(GetPixelPosition().x + Globals::gGridX + GetWidth() / 2, GetPixelPosition().y + GetHeight() / 2),
		GetRange() * GRID_SQUARE_SIZE, color);
}

void Tower::OnUpdate()
{
	if (Globals::gIO->MouseClicked[ImGuiMouseButton_Left])
	{
		ImVec2 mouseClickedPos = Globals::gIO->MouseClickedPos[ImGuiMouseButton_Left];
		// If the tower is being clicked
		int32_t pixelX = mouseClickedPos.x - Globals::gGridX, pixelY = mouseClickedPos.y - Globals::gGridY;
		uint8_t tileX, tileY;
		Globals::gGame->GetPlayField()->GetGridPositionFromPixels(pixelX, pixelY, tileX, tileY);
		if (tileX == GetTilePosition().x && tileY == GetTilePosition().y)
		{
		    
		}
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
