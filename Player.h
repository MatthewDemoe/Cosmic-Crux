#pragma once
#include <vector>
#include "Timer.h"
#include "Projectile.h"
#include "Enemy.h"


class Player : public GameObject
{
public:
	friend class Game;
	Player();
	~Player();

	//Getting input from the controller
	void xin();
	void shoot();
	void update(std::vector<Enemy*> enemies);

	XBox::Stick getLStick();
	XBox::Stick getRStick();

	std::vector<Projectile*> getProjectiles();
	void setNum(int num);
	Projectile projectile;
	void deleteProjectile(int index);
	Timer* updateTimer = nullptr;

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