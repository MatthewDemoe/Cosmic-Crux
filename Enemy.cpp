#include "Enemy.h"

Enemy::Enemy()
{
	updateTimer = new Timer();
	updateTimer->tick();
}

Enemy::~Enemy()
{

}

std::vector<Projectile*> Enemy::getProjectiles()
{
	return projectiles;
}
