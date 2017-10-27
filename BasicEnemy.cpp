#include "BasicEnemy.h"

BasicEnemy::BasicEnemy()
{
	velocity = glm::vec2(0.1f, -0.025f);
	delay = 1.0f;
	spawnDelay = 5.0f;
}

BasicEnemy::~BasicEnemy()
{

}

void BasicEnemy::update(std::vector<Player*> players)
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
		if(projectiles[i]->isOffscreen())
		{
			projectiles.erase(projectiles.begin() + i);
			break;
		}
	}
}

void BasicEnemy::shoot(std::vector<Player*> players)
{
	//Projectile pointer to be pushed back into the vector of projectiles
	Projectile* temp = new Projectile();

	//Set the mesh of the new projectile
	temp->mesh = projectile.mesh;
	temp->tex = projectile.tex;

	//Make the new projectile's starting location equal to the player's 
	//temp->transform = transform;
	temp->location = glm::vec2(0.0f, 0.0f);
	temp->move(location.x, location.y);

	//Normalize the projectile's velocity vector so that projectiles will fire at the same speed regardless of the amount of tilt amount
	glm::vec2 enemyToPlayer = glm::vec2(players[0]->location.x - location.x, players[0]->location.y - location.y);
	float length = sqrt((enemyToPlayer.x * enemyToPlayer.x) + (enemyToPlayer.y * enemyToPlayer.y));
	enemyToPlayer.x /= length;
	enemyToPlayer.y /= length;

	if (enemyToPlayer.x <= 0.0f)
	{
		temp->rotate = glm::rotate(temp->rotate, acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	else
	{
		temp->rotate = glm::rotate(temp->rotate, -acos(enemyToPlayer.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	//Assign velovity
	temp->velocity = glm::vec2(0.2 * enemyToPlayer.x, 0.2 * enemyToPlayer.y);

	projectiles.push_back(temp);
}