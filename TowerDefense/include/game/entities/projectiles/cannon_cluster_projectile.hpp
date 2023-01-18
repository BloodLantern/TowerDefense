#pragma once

#include "cannon_ball_projectile.hpp"

class CannonClusterProjectile : public CannonBallProjectile
{
public:
    CannonClusterProjectile(bool recursive);
    CannonClusterProjectile(float_t speed, uint32_t damage, float_t lifetime, float_t scale, Vector2 velocity);

    Projectile* Clone() const override;

    virtual void Explode() override;

private:
    bool mRecursive;
};
