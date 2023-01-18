#pragma once

#include "tower.hpp"
#include "bee_projectile.hpp"

class BeehiveTower : public Tower
{
public:
	BeehiveTower(Texture* texture);
	~BeehiveTower();

	Tower* Clone() const override;
	void Shoot() override;
	void OnUpdate() override;

    void OnCustomUpgrade() override;
	const char* const GetCustomUpgradeTooltip(uint8_t level) const override;
	
	void RemoveBee(BeeProjectile* bee);

private:
	uint8_t mAmountOfBees;
	uint8_t mMaxAmountOfBees;

	std::vector<BeeProjectile*> mBees;
};
