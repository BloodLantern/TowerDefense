#include "Entity.h"
#include "PlayField.h"
#include "Globals.h"

Entity::Entity()
{
	setPixelPosition(Point2());
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

	m_tilePosition = newPixelPosition / GRID_SQUARE_SIZE;
};

void Entity::setTilePosition(Point2 newTilePosition)
{
	m_tilePosition = newTilePosition;
	
	m_pixelPosition = newTilePosition * GRID_SQUARE_SIZE + GRID_SQUARE_SIZE / 2.f;
}
