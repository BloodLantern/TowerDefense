#pragma once

#include "tower.hpp"

class BeehiveTower : public Tower
{
public:
	BeehiveTower(Texture* texture);

	void OnCustomUpgrade() override;
	const char* const GetCustomUpgradeTooltip(uint8_t level) const override;
	
	void OnUpdate() override;
	void Shoot() override;

	Tower* Clone() const override;

private:
	uint32_t mCashBonus;
	uint8_t mMaxCashGenerations;

	float_t mCashGenerationTimer;
	uint8_t mAmoutOfCashGenerated;
	
	void TryBuffBeenests();
};

