#pragma once

#include "tower.hpp"

class CannonTower : public Tower
{
public:
    CannonTower(Texture* texture);

    Tower* Clone() const override;

private:
    void Shoot() override;
};
