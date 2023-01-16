#pragma once

#include "projectile.hpp"

class CannonBallProjectile : public Projectile
{
public:
    CannonBallProjectile();

    Projectile* Clone() const override;

    void OnUpdate() override;
    void OnRender() override;

private:
    uint8_t mExplodeAnimation = 0;

    void HandleEnemyCollision() override;
};
