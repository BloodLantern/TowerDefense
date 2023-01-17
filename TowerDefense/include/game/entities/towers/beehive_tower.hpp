#pragma once

#include "tower.hpp"

class BeehiveTower : public Tower
{
public:
	BeehiveTower(Texture* texture);

	Tower* Clone() const override;
	void Shoot() override;
	void OnUpdate() override;

    void OnCustomUpgrade() override;
    const char* GetCustomUpgradeTooltip(uint8_t level) const override { return nullptr; }
	
	void RemoveBee();

private:
	uint8_t mAmountOfBees;
	uint8_t mMaxAmountOfBees;
};
