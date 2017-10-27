#include <iostream>
#include <math.h>
#include "Player.h"

Player::Player()
{
	updateTimer = new Timer();
	updateTimer->tick();
}

Player::~Player()
{
	//Delete pointers
	for (int i = 0; i < projectiles.size(); i++)
	{
		delete projectiles[i];
	}
}

void Player::update(std::vector<Enemy*> enemies)
{
	updateTimer->tick();
	xin();

	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->move(projectiles[i]->getVelocity().x, projectiles[i]->getVelocity().y);

		//Erase the projectile if it leaves the screen 
		if ((projectiles[i]->location.x >= 10) || (projectiles[i]->location.y >= 10)
			|| (projectiles[i]->location.x <= -10) || (projectiles[i]->location.y <= -10))
		{
			deleteProjectile(i);
			break;
		}

		//Iterate through each enemy, check if current projectile is intersecting with it
		for (int j = 0; j < enemies.size(); j++)
		{
			if (projectiles[i]->collide(*enemies[j]))
			{
				//Erase projectile, Erase enemy and spawn in a new location for now. 
				deleteProjectile(i);

				float x = (rand() % 10) - 5;
				float y = (rand() % 10) - 5;

				enemies[j]->move(-enemies[j]->location.x, -enemies[j]->location.y);
				enemies[j]->move(x, y);
			}
		}
	}
}

//Getting input from the controller
void Player::xin()
{
	//Used for shooting delay
	localTime += updateTimer->getElapsedTimeS();
	
	//poll controller
	controller.DownloadPackets(2);
	controller.GetSticks(playerNum, lStick, rStick);

	//Move the controller in the x axis based on the left stick's x axis, and they y likewise
	move(lStick.xAxis * 0.25f, lStick.yAxis * 0.25f);

	//Checking if the right stick is tilted more than a certain amount. tilted will be true if the right stick is being tilted.
	bool tilted = std::abs(rStick.xAxis) > 0.25f || std::abs(rStick.yAxis) > 0.25f ? true : false;

	//Shoot after they delay, and reset delay
	if (tilted && (localTime > delay))
	{
		localTime = 0;
		
		shoot();
	}

	if (controller.GetButton(0, XBox::A)) {
		std::cout << "A Pressed" << std::endl;
	}
}

//Getters
XBox::Stick Player::getLStick()
{
	return lStick;
}

XBox::Stick Player::getRStick()
{
	return rStick;
}

std::vector<Projectile*> Player::getProjectiles()
{
	return projectiles;
}

void Player::setNum(int num)
{
	playerNum = num;
}

void Player::shoot()
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
	glm::vec2 normalVel = glm::vec2(rStick.xAxis, rStick.yAxis);
	float length = sqrt((normalVel.x * normalVel.x) + (normalVel.y * normalVel.y));
	normalVel.x /= length;
	normalVel.y /= length;

	//Essentially finding and rotating by the dot product with the unit y vector (0, 1)... It will just return normalVel.y
	//Rotation value will always be between 0 and 180, so we need to just check which x direction the projectile is going to fix rotation
	if (normalVel.x <= 0.0f)
	{
		temp->rotate = glm::rotate(temp->rotate, acos(normalVel.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	else
	{
		temp->rotate = glm::rotate(temp->rotate, -acos(normalVel.y), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	//Assign velovity
	temp->velocity = glm::vec2(0.2 * normalVel.x, 0.2 * normalVel.y);

	projectiles.push_back(temp);
}

void Player::deleteProjectile(int index)
{
	//delete projectiles[index];
	projectiles.erase(projectiles.begin() + index);
	std::cout << "Deleted prjectile : " << index << std::endl;
}