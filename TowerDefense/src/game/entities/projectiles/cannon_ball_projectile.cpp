#include "cannon_ball_projectile.hpp"
#include "globals.hpp"

#define CANNON_BALL_PROJECTILE_EXPLOSION_RADIUS (4 * GRID_SQUARE_SIZE)
#define CANNON_BALL_PROJECTILE_EXPLOSION_ANIMATION_TIME 20

CannonBallProjectile::CannonBallProjectile()
    : Projectile(15, 2, 40, 0.5f)
{
    mScale = 0.05f;
    mTexture = Globals::gResources->GetTexture("projectiles\\cannon_ball");
}

Projectile* CannonBallProjectile::Clone() const
{
    return new CannonBallProjectile();
}

void CannonBallProjectile::HandleEnemyCollision()
{
	// For each enemy
	for (std::vector<Enemy*>::iterator it = Globals::gGame->enemies.begin(); it != Globals::gGame->enemies.end(); ++it)
	{
        // If an enemy was hit
		if (Vector2((*it)->GetPixelPosition(), GetPixelPosition()).GetSquaredNorm() < PROJECTILE_COLLISION_RADIUS * PROJECTILE_COLLISION_RADIUS)
        {
            Explode();
            break;
        }
	}
}

void CannonBallProjectile::OnUpdate()
{
    // If not in explosion animation
    if (mExplodeAnimation == 0)
    {
        // If the projectile lifetime is over, destroy it
        if (mLifetime <= 0.f)
        {
            Explode();
            return;
        }

        // Update velocity if the projectile has a target
        if (mTarget && !mTarget->toDelete)
            mVelocity = Vector2(GetPixelPosition(), mTarget->GetPixelPosition()).Normalize() * 60;

        // If the projectile hits an enemy, deal its damage and reduce its pierce
        HandleEnemyCollision();

        // Update its position
        SetPixelPosition(GetPixelPosition() + mVelocity * mSpeed * Globals::gGame->GetPlayingSpeedDeltaTime());


        // Update lifetime
        mLifetime -= Globals::gGame->GetPlayingSpeedDeltaTime();
    }
}

void CannonBallProjectile::OnRender()
{
	Point2 pixelPosition(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y);

    if (mExplodeAnimation == 0)
    {
        ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *mTexture, pixelPosition, mScale, mRotation);
        mRotation += 10.f * Globals::gGame->GetPlayingSpeedDeltaTime();
    }
    // Projectile exploded
    else
    {
	    Globals::gDrawList->AddCircleFilled(pixelPosition, 10 + 3 * mExplodeAnimation, IM_COL32(0x80, 0x08, 0x0, 0x50));
        if (mExplodeAnimation++ >= CANNON_BALL_PROJECTILE_EXPLOSION_ANIMATION_TIME)
            toDelete = true;
    }
}

void CannonBallProjectile::Explode()
{
    // For each enemy in explosion range
    for (std::vector<Enemy*>::iterator it2 = Globals::gGame->enemies.begin(); it2 != Globals::gGame->enemies.end(); ++it2)
    {
        Enemy* inRangeEnemy = *it2;
        if (Vector2(inRangeEnemy->GetPixelPosition(), GetPixelPosition()).GetSquaredNorm() < CANNON_BALL_PROJECTILE_EXPLOSION_RADIUS * CANNON_BALL_PROJECTILE_EXPLOSION_RADIUS)
        {
            uint32_t damageDealt;
            // If the enemy died, update tower kill stat
            if (inRangeEnemy->DealDamage(mDamage, damageDealt))
            {
                if (mOwner)
                {
                    mOwner->IncreaseKillCount(1);
                    mOwner->IncreaseMoneyGenerated(inRangeEnemy->GetMoneyDrop());
                }
            }
            if (mOwner)
                mOwner->IncreaseDamageDealt(damageDealt);

            // If all the pierce was used, stop the loop
            if (mPierce == 0)
                break;
            mPierce--;
        }
    }

    // The projectile exploded, show the animation and delete it
    mExplodeAnimation = 1;
}
