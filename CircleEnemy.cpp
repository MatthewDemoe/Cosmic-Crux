#include "CircleEnemy.h"

CircleEnemy::CircleEnemy()
{
	velocity = glm::vec2(0.1f, -0.025f);
	delay = 2.0f;
	spawnDelay = 20.0f;
}

CircleEnemy::~CircleEnemy()
{

}

void CircleEnemy::update(std::vector<Player*> players)
{
	//Used for shooting delay
	updateTimer->tick();
	localTime += updateTimer->getElapsedTimeS();

	//Shoot after they delay, and reset delay
	if (localTime > delay)
	{
		localTime = 0;

		shoot(players);
	}

	move(velocity.x, velocity.y);

	if ((location.x >= 20.0f) || (location.x <= -20.0f))
		velocity.x = -velocity.x;

	for (int i = 0; i < getProjectiles().size(); i++)
	{
		projectiles[i]->move(projectiles[i]->getVelocity().x, projectiles[i]->getVelocity().y);
		if (projectiles[i]->isOffscreen())
		{
			projectiles.erase(projectiles.begin() + i);
			break;
		}
	}
}

void CircleEnemy::shoot(std::vector<Player*> players)
{
	float increment = 3.14f / 4.0f;
	for (float i = 0; i < 8; i++)
	{
		Projectile* temp = new Projectile();
		temp->mesh = projectile.mesh;
		temp->tex = projectile.tex;

		temp->location = glm::vec2(0.0f, 0.0f);
		temp->move(location.x, location.y);


		glm::vec2 dir = glm::vec2(cos(increment * i), sin(increment * i));
		float length = sqrt((dir.x * dir.x) + (dir.y * dir.y));
		dir.x /= length;
		dir.y /= length;

		if (dir.x <= 0.0f)
		{
			temp->rotate = glm::rotate(temp->rotate, acos(dir.y), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		else
		{
			temp->rotate = glm::rotate(temp->rotate, -acos(dir.y), glm::vec3(0.0f, 0.0f, 1.0f));
		}

		//Assign velovity
		temp->velocity = glm::vec2(0.2 * dir.x, 0.2 * dir.y);

		projectiles.push_back(temp);
	}
}
