#pragma once

#include "enemy.hpp"
#include <Point2.h>

class AntColonyEnemy : public Enemy
{
public:
	AntColonyEnemy(Point2 pos);
	~AntColonyEnemy();
    
	void OnUpdate() override;

private:
    const Texture* mHealthBar;
	float_t mAntSpawnAroundCooldown = 0;
	float_t mAntSpawnCooldown = 0;

	void SpawnAntsAround(uint8_t count);
	void SpawnAnt();
    void DrawHealthBar(ImVec2& pos) override;
};
