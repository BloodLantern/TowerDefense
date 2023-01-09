#include "Entity.h"
#include "PlayField.h"
#include "Globals.h"

Entity::Entity()
{
	mPixelPosition = Point2(INFINITY, INFINITY);
	mTilePosition = Point2(INFINITY, INFINITY);
}

Entity::Entity(const Entity& other)
    : mTexture(other.mTexture)
{
}

Entity::Entity(Point2 pixelPosition)
{
	SetPixelPosition(pixelPosition);
}

Entity::Entity(float_t pixelPositionX, float_t pixelPositionY)
{
	SetPixelPosition(Point2(pixelPositionX, pixelPositionY));
}

Entity::Entity(int32_t tilePositionX, int32_t tilePositionY)
{
	SetTilePosition(Point2((float_t)tilePositionX, (float_t)tilePositionY));
}

void Entity::Draw()
{
	ImVec2 pos(Globals::gWindowX + mPixelPosition.x, Globals::gWindowY + mPixelPosition.y);

	Globals::gDrawList->AddCircleFilled(pos, GRID_SQUARE_SIZE / 2, IM_COL32(0xFF, 0xFF, 0x0, 0xFF));
}

void Entity::SetPixelPosition(Point2 newPixelPosition)
{
	mPixelPosition = newPixelPosition;

	uint8_t tileX, tileY;
	Globals::gGame->GetPlayField()->GetGridPositionFromPixels(newPixelPosition.x, newPixelPosition.y, tileX, tileY);
	mTilePosition = Point2(tileX, tileY);
}

void Entity::SetTilePosition(Point2 newTilePosition)
{
	mTilePosition = newTilePosition;

	int32_t pixelX, pixelY;
	Globals::gGame->GetPlayField()->GetPixelPositionFromGrid(newTilePosition.x, newTilePosition.y, pixelX, pixelY);
	mPixelPosition = Point2(pixelX, pixelY);
}
