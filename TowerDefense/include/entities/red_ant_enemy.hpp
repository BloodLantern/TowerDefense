#pragma once

#include "enemy.hpp"

class RedAntEnemy : public Enemy
{
public:
	RedAntEnemy(Point2 pos);
	~RedAntEnemy();
    
	void OnRender() override;
};
