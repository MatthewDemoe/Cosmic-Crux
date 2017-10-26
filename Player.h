#pragma once
#include <vector>
#include "Projectile.h"


class Player : public GameObject
{
public:
	Player();
	~Player();

	//Getting input from the controller
	void xin();
	void shoot();

	XBox::Stick getLStick();
	XBox::Stick getRStick();

	std::vector<Projectile*> getProjectiles();
	void setNum(int num);
	Projectile projectile;
	void deleteProjectile(int index);

private:
	XBox::XBoxInput controller;
	XBox::Stick lStick;
	XBox::Stick rStick;
	int playerNum;
	float delay = 0.25f;
	float localTime = 0.0f;


	//Vector containing all the projectiles the player has fired. Stored in player for collision purposes. 
	std::vector<Projectile*> projectiles;
};