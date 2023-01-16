#include "cannon_ball_projectile.hpp"
#include "globals.hpp"

#define CANNON_BALL_PROJECTILE_EXPLOSION_RADIUS 100.f

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
		if (Vector2((*it)->GetPixelPosition(), GetPixelPosition()).GetSquaredNorm() < PROJECTILE_COLLISION_RADIUS * 20.f)
        {
            // For each enemy in explosion range
	        for (std::vector<Enemy*>::iterator it2 = Globals::gGame->enemies.begin(); it2 != Globals::gGame->enemies.end(); ++it2)
            {
		        Enemy* inRangeEnemy = *it2;
		        if (Vector2(inRangeEnemy->GetPixelPosition(), GetPixelPosition()).GetSquaredNorm() < CANNON_BALL_PROJECTILE_EXPLOSION_RADIUS * 20.f)
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

                    // If all the pierce was used, destroy the projectile
                    if (mPierce == 0)
                    {
                        toDelete = true;
                        return;
                    }
                    mPierce--;
                }
            }
            break;
        }
	}
}

void CannonBallProjectile::OnRender()
{
	Point2 pixelPosition(Globals::gGridX + GetPixelPosition().x, Globals::gGridY + GetPixelPosition().y);
	Globals::gDrawList->AddCircleFilled(pixelPosition, 5, IM_COL32(0x0, 0x0, 0x0, 0xFF));
}
