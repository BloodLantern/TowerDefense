#pragma once

#include "tower.hpp"
#include "bee_projectile.hpp"
#include "particle_emitter.hpp"

class BeenestTower : public Tower
{
public:
	BeenestTower(Texture* texture);
	~BeenestTower();

	Tower* Clone() const override;
	void Shoot() override;
	void OnUpdate() override;

    void OnCustomUpgrade() override;
	const char* const GetCustomUpgradeTooltip(uint8_t level) const override;
	
	void RemoveBee(BeeProjectile* bee);

	bool IsBuffed();
	void ApplyBeehiveBuff();

private:
	uint8_t mAmountOfBees;
	uint8_t mMaxAmountOfBees;

	std::vector<BeeProjectile*> mBees;

	bool mBuffed;
	ParticleEmitter* mBuffParticlesEmitter;
};
