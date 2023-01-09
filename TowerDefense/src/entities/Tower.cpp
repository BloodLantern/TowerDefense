#include "Tower.h"

#include "Globals.h"

Tower::Tower(const Tower& other)
	: Entity(other),
	m_range(other.m_range),
	m_attackSpeed(other.m_attackSpeed),
	m_projectileTemplate(other.m_projectileTemplate),
	m_width(other.m_width),
	m_height(other.m_height)
{
}

Tower::Tower(Projectile* projectileTemplate)
	: Entity(), m_projectileTemplate(*projectileTemplate)
{

}

Tower::Tower(Point2 pixelPosition, float_t range, float_t attackSpeed, Projectile* projectileTemplate)
	: Entity(pixelPosition), m_range(range), m_attackSpeed(attackSpeed), m_projectileTemplate(*projectileTemplate)
{
	m_customUpgradeLevel = 0;
	m_genericUpgradeLevel = 0;

	m_target = nullptr;
}

void Tower::shoot(const Projectile& projTemplate)
{

}

void Tower::draw()
{
	ImVec2 topLeft = ImVec2(Globals::gGridX + getPixelPosition().x, Globals::gGridY + getPixelPosition().y);
	ImVec2 bottomRight = ImVec2(topLeft.x + getWidth() * GRID_SQUARE_SIZE, topLeft.y + getHeight() * GRID_SQUARE_SIZE);
	Globals::gDrawList->AddImage(getTexture()->id, topLeft, bottomRight);
}
