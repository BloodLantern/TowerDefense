#include "Entity.h"
#include "PlayField.h"
#include "Globals.h"

Entity::Entity()
{
	m_pixelPosition = Point2(INFINITY, INFINITY);
	m_tilePosition = Point2(INFINITY, INFINITY);
};

Entity::Entity(Point2 pixelPosition)
{
	setPixelPosition(pixelPosition);
};

Entity::Entity(float_t pixelPositionX, float_t pixelPositionY)
{
	setPixelPosition(Point2(pixelPositionX, pixelPositionY));
};

Entity::Entity(int32_t tilePositionX, int32_t tilePositionY)
{
	setTilePosition(Point2((float_t)tilePositionX, (float_t)tilePositionY));
};

void Entity::draw()
{
	ImVec2 pos(Globals::gWindowX + m_pixelPosition.x, Globals::gWindowY + m_pixelPosition.y);

	Globals::gDrawList->AddCircleFilled(pos, GRID_SQUARE_SIZE / 2, IM_COL32(0xFF, 0xFF, 0x0, 0xFF));
};

void Entity::setPixelPosition(Point2 newPixelPosition)
{
	m_pixelPosition = newPixelPosition;

	uint8_t tileX, tileY;
	Globals::gGame->getPlayField()->getGridPositionFromPixels(newPixelPosition.x, newPixelPosition.y, tileX, tileY);
	m_tilePosition = Point2(tileX, tileY);
};

void Entity::setTilePosition(Point2 newTilePosition)
{
	m_tilePosition = newTilePosition;

	int32_t pixelX, pixelY;
	Globals::gGame->getPlayField()->getPixelPositionFromGrid(newTilePosition.x, newTilePosition.y, pixelX, pixelY);
	m_tilePosition = Point2(pixelX, pixelY);
}
