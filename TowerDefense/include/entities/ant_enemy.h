#pragma once

#include "enemy.hpp"

class AntEnemy : public Enemy
{
public:
	AntEnemy(Point2 pos);
	~AntEnemy();
};
