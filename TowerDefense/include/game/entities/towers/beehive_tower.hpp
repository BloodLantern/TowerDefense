#pragma once

#include "tower.hpp"

class BeehiveTower : public Tower
{
public:
	BeehiveTower(Texture* texture);

	Tower* Clone() const override;
	void Shoot() override;
	void OnUpdate() override;
	
	void RemoveBee();

private:
	uint8_t mAmountOfBees;
	uint8_t mMaxAmountOfBees;
};
