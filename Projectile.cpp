#include "Projectile.h"

Projectile::Projectile()
{
	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

Projectile::Projectile(float x, float y) 
{
	velocity.x = x;
	velocity.y = y;
}

Projectile::~Projectile()
{

}

glm::vec2 Projectile::getVelocity()
{
	return velocity;
}

void Projectile::SetVelocity(float x, float y)
{
	velocity.x = x;
	velocity.y = y;
}