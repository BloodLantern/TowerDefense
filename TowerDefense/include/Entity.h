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

	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;

	bool IsOnGrid() const;
	inline void SnapToGrid() { SetTilePosition(GetTilePosition()); };

	Point2 GetPixelPosition() const { return mPixelPosition; };
	void SetPixelPosition(Point2 newPixelPosition);

	Point2 GetTilePosition() const { return mTilePosition; };
	void SetTilePosition(Point2 newTilePosition);

	float_t GetScale() const { return mScale; };
	void SetScale(float_t newScale) { mScale = newScale; }
	void AddScale(float_t scale) { mScale += scale; };

	float_t GetRotation() const { return mRotation; }
	void SetRotation(float_t newRotation) { mRotation = newRotation; }
	void AddRotation(float_t rotation) { mRotation += rotation; }
	
	Texture* GetTexture() const { return mTexture; }
	void SetTexture(Texture* newTexture) { mTexture = newTexture; }

protected:

private:
	Point2 mPixelPosition;
	Point2 mTilePosition;
	float_t mScale = 1;
	float_t mRotation = 0;

	Texture* mTexture = nullptr;
};

