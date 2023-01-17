#pragma once

#include "enemy.hpp"

class LadybugEnemy : public Enemy
{
public:
	LadybugEnemy(Point2 pos);
	~LadybugEnemy();

	void OnUpdate() override;
};

