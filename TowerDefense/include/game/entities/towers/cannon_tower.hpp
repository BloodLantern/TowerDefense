#pragma once

#include "tower.hpp"

class CannonTower : public Tower
{
public:
    CannonTower(Texture* texture);

    void OnCustomUpgrade() override;

    Tower* Clone() const override;

private:
    Texture* mHandleTexture;
    Texture* mCannonTexture;

    void Draw(const ImVec2& topLeft, const ImVec2& bottomRight) const override;
};
