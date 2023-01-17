#pragma once

#include "tower.hpp"

class CannonTower : public Tower
{
public:
    CannonTower(Texture* texture);

    Tower* Clone() const override;

private:
    void Draw(const ImVec2& topLeft, const ImVec2& bottomRight) const override;
};
