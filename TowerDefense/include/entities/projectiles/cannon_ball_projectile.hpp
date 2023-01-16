#pragma once

#include "projectile.hpp"

class CannonBallProjectile : public Projectile
{
public:
    CannonBallProjectile();

    Projectile* Clone() const override;

    void OnRender() override;

private:
    void HandleEnemyCollision() override;
};
