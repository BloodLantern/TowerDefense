#include "cannon_tower.hpp"

CannonTower::CannonTower(Texture* texture)
    : Tower(new Projectile(15, 2, 40, 0.5f), 0.2f, 5.5f, "Cannon", 200, texture)
{

}