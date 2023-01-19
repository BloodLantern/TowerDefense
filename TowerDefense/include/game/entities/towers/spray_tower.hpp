#pragma once

#include "tower.hpp"

class SprayTower : public Tower
{
public:
	SprayTower(Texture* texture);

	void OnCustomUpgrade() override;

	const char* const GetCustomUpgradeTooltip(uint8_t level) const override;
	void Shoot() override;
	Tower* Clone() const override;

private:
	uint8_t mProjAmount;
};

