#pragma once

#include "imgui_utils.hpp"

#include <Maths/Point2.h>
#include <stdint.h>

class Entity
{
public:
	Entity();
	Entity(const Entity& other);
	Entity(Point2 pixelPosition);
	Entity(float_t pixelPositionX, float_t pixelPositionY);
	Entity(int32_t tilePositionX, int32_t tilePositionY);
	virtual ~Entity() {};

	virtual void Draw() = 0;

	Point2 GetPixelPosition() { return mPixelPosition; };
	void SetPixelPosition(Point2 newPixelPosition);

	Point2 GetTilePosition() { return mTilePosition; };
	void SetTilePosition(Point2 newTilePosition);

	float_t GetScale() { return mScale; };
	void SetScale(float_t newScale) { mScale = newScale; }
	void AddScale(float_t scale) { mScale += scale; };

	float_t GetRotation() { return mRotation; }
	void SetRotation(float_t newRotation) { mRotation = newRotation; }
	void AddRotation(float_t rotation) { mRotation += rotation; }
	
	Texture* GetTexture() const { return mTexture; }

protected:
	void SetTexture(Texture* newTexture) { mTexture = newTexture; }

private:
	Point2 mPixelPosition;
	Point2 mTilePosition;
	float_t mScale = 1;
	float_t mRotation = 0;

	Texture* mTexture = nullptr;
};

