#pragma once
#include "GameObject.h"

class Projectile : public GameObject
{
public:
	friend class Player;
	Projectile();
	//Initialize the projectile with an x and y velocity
	Projectile(float x, float y);
	~Projectile();

	glm::vec2 getVelocity();
	void SetVelocity(float, float);

private:
	//Projectiles will be given an x and y velocity based on the direction the right controller stick is tilted. 
	glm::vec2 velocity;
};