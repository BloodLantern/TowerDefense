#pragma once

#include "imgui_utils.hpp"

#include <Maths/Point2.h>
#include <stdint.h>

class Entity
{
public:
	Entity();
	Entity(Point2 pixelPosition);
	Entity(float_t pixelPositionX, float_t pixelPositionY);
	Entity(int32_t tilePositionX, int32_t tilePositionY);
	~Entity() {};

	virtual void draw() = 0;

	Point2 getPixelPosition() { return m_pixelPosition; };
	void setPixelPosition(Point2 newPixelPosition);

	Point2 getTilePosition() { return m_tilePosition; };
	void setTilePosition(Point2 newTilePosition);

	float_t getScale() { return m_scale; };
	void setScale(float_t newScale) { m_scale = newScale; }
	void addScale(float_t scale) { m_scale += scale; };

	float_t getRotation() { return m_rotation; }
	void setRotation(float_t newRotation) { m_rotation = newRotation; }
	void addRotation(float_t rotation) { m_rotation += rotation; }
	
	Texture* getTexture() const { return m_texture; }

protected:
	void setTexture(Texture* newTexture) { m_texture = newTexture; }

private:
	Point2 m_pixelPosition;
	Point2 m_tilePosition;
	float_t m_scale = 1;
	float_t m_rotation = 0;

	Texture* m_texture;
};

