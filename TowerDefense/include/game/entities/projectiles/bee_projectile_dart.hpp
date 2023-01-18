#pragma once

#include "projectile.hpp"

class BeeProjectileDart : public Projectile
{
public:
	BeeProjectileDart(Point2 position, Vector2 velocity);

	void OnUpdate() override;
	void OnRender() override;
};
