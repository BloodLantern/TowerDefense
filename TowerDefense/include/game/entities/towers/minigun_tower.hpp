#pragma once

#include "tower.hpp"

class MinigunTower : public Tower
{
public:
	MinigunTower(Texture* texture);

    void OnUpdate() override;

	Tower* Clone() const override;
	void Shoot() override;

    void OnCustomUpgrade() override;
    const char* const GetCustomUpgradeTooltip(uint8_t level) const override;

private:
    Texture* mHandleTexture;
    Texture* mMinigunTexture;
    Texture* mFlameTexture;
    uint8_t mFlameAlpha = 0;
    
    void DrawTowerBarTexture() override;
    void Draw(const ImVec2& topLeft, const ImVec2& bottomRight) const override;
};
