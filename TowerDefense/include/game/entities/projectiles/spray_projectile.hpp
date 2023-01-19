#pragma once

#include "projectile.hpp"

class SprayProjectile : public Projectile
{
public:
	SprayProjectile();

	void OnRender() override;
	Projectile* Clone() const override;
};
