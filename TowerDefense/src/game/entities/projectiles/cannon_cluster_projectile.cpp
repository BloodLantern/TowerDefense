#include "cannon_cluster_projectile.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "globals.hpp"

#define CLUSTER_BOMB_COUNT 8

CannonClusterProjectile::CannonClusterProjectile(bool recursive)
    : CannonBallProjectile(), mRecursive(recursive)
{
    mDamage = 10;
    //TODO: mTexture = RED BOMB
}

CannonClusterProjectile::CannonClusterProjectile(float_t speed, uint32_t damage, float_t lifetime, float_t scale, Vector2 velocity)
    : CannonBallProjectile(speed, damage, lifetime, scale, velocity), mRecursive(false)
{
    //mTexture = RED BOMB
}

Projectile *CannonClusterProjectile::Clone() const
{
    CannonClusterProjectile* clone = new CannonClusterProjectile(mRecursive);
    clone->mSpeed = mSpeed;
    return clone;
}

void CannonClusterProjectile::Explode()
{
    CannonBallProjectile::Explode();

    // Create cannon balls in 8 directions
    const Vector2 up = Vector2(0, -1) * 100;
    const float_t dangle = (float_t) M_PI * 2.f / CLUSTER_BOMB_COUNT;

    uint16_t pierce = mPierce / CLUSTER_BOMB_COUNT;

    for (int i = 0; i < CLUSTER_BOMB_COUNT; i++)
    {
        Projectile* proj = nullptr;
        if (mRecursive)
            proj = new CannonClusterProjectile(2, 7, 0.3f, 0.03f, up.Rotate(dangle * i));
        else
            proj = new CannonBallProjectile(2, 5, 0.3f, 0.03f, up.Rotate(dangle * i));
        proj->SetPierce(pierce);
        proj->SetPixelPosition(GetPixelPosition());
        proj->SetOwner(mOwner);
        Globals::gGame->projectilesQueue.push_back(proj);
    }
}
