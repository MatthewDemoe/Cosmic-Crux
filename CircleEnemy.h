#pragma once
#include "Enemy.h"

class CircleEnemy : public Enemy
{
public:
	CircleEnemy();
	~CircleEnemy();

	void update(std::vector<Player*>);
	void shoot(std::vector<Player*>);

};