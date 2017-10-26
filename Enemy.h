#pragma once
#include "GameObject.h"
#include "Projectile.h"
#include "ShaderProgram.h"
#include <vector>
#include <glm/gtc/type_ptr.hpp>


class Enemy : public GameObject
{
public:
	Enemy();
	~Enemy();
	void update(float);
	void shoot();
	void drawProjectiles(ShaderProgram &shader);

	Projectile projectile;


private:
	std::vector<Projectile*> projectiles;
	float fireRate;
	float fireTimer = 0;
};