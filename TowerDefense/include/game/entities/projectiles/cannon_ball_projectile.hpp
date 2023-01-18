#pragma once

#include "projectile.hpp"

class CannonBallProjectile : public Projectile
{
public:
    CannonBallProjectile();
    CannonBallProjectile(float_t speed, uint32_t damage, float_t lifetime, float_t scale, Vector2 velocity);

    virtual Projectile* Clone() const override;

    void OnUpdate() override;
    void OnRender() override;

    virtual void Explode();
    
private:
    uint8_t mExplodeAnimation = 0;
    bool mCollidesWithEnemies = true;

    void HandleEnemyCollision() override;
};
