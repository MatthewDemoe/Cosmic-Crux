#include <iostream>
#include <math.h>
#include "Player.h"

Player::Player()
{

}

Player::~Player()
{
	//Delete pointers
	for (int i = 0; i < projectiles.size(); i++)
	{
		delete projectiles[i];
	}
}

//Getting input from the controller
void Player::xin()
{
	//Used for shooting delay
	localTime += 0.01f;
	
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

	//Make the new projectile's starting location equal to the player's 
	temp->transform = transform;
	
	temp->location = location;

	//Normalize the projectile's velocity vector so that projectiles will fire at the same speed regardless of the amount of tilt amount
	glm::vec2 normalVel = glm::vec2(rStick.xAxis, rStick.yAxis);
	float length = sqrt((normalVel.x * normalVel.x) + (normalVel.y * normalVel.y));
	normalVel.x /= length;
	normalVel.y /= length;
	
	//Essentially finding and rotating by the dot product with the unit y vector (0, 1)... It will just return normalVel.y
	//Rotation value will always be between 0 and 180, so we need to just check which x direction the projectile is going to fix rotation
	if (normalVel.x >= 0.0f)
	{
		temp->rotate = glm::rotate(temp->transform, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		temp->rotate = glm::rotate(temp->rotate, -acos(normalVel.y), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	else
	{
		temp->rotate = glm::rotate(temp->transform, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		temp->rotate = glm::rotate(temp->rotate, acos(normalVel.y), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	//Assign velovity
	temp->velocity = glm::vec2(0.1 * normalVel.x, 0.1 * normalVel.y);

	projectiles.push_back(temp);
}

void Player::deleteProjectile(int index)
{
	//delete projectiles[index];
	projectiles.erase(projectiles.begin() + index);
	std::cout << "Deleted prjectile : " << index << std::endl;
}