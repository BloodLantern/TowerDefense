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
	float_t mAntSpawnCooldown = 0;

	void SpawnAnts(uint8_t count);
    void DrawHealthBar(ImVec2& pos) override;
};
