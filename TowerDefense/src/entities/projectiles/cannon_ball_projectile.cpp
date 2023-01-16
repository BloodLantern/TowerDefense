#include "cannon_ball_projectile.hpp"
#include "globals.hpp"

#define CANNON_BALL_PROJECTILE_EXPLOSION_RADIUS (4 * GRID_SQUARE_SIZE)
#define CANNON_BALL_PROJECTILE_EXPLOSION_ANIMATION_TIME 20

CannonBallProjectile::CannonBallProjectile()
    : Projectile(15, 2, 40, 0.5f)
{
}

Projectile *CannonBallProjectile::Clone() const
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
            break;
        }
	}
}

void CannonBallProjectile::OnUpdate()
{
    if (mExplodeAnimation == 0)
        Projectile::OnUpdate();
}

void CannonBallProjectile::OnRender()
{
	Point2 pixelPosition(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y);

    if (mExplodeAnimation == 0)
    {
	    Globals::gDrawList->AddCircleFilled(pixelPosition, 5, IM_COL32(0x0, 0x0, 0x0, 0xFF));
    }
    // Projectile exploded
    else
    {
	    Globals::gDrawList->AddCircleFilled(pixelPosition, 10 + 3 * mExplodeAnimation, IM_COL32(0x80, 0x08, 0x0, 0x50));
        if (mExplodeAnimation++ >= CANNON_BALL_PROJECTILE_EXPLOSION_ANIMATION_TIME)
            toDelete = true;
    }
}
