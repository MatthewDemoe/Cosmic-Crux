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
	void xin(Player* otherPlayer);
	void shoot();
	void update(std::vector<Enemy*>* enemies, Player* otherPlayer);

	XBox::Stick getLStick(); 
	XBox::Stick getRStick();

	std::vector<Projectile*> getProjectiles();
	void setNum(int num);
	Projectile projectile;
	void deleteProjectile(int index);
	Timer* updateTimer = nullptr;

	bool getTransform() { return isTransformed; }
	bool isAlive() { return alive; }

	float progress = 0;
	float transformMax = 15;

	GameObject blackBar;
	GameObject yellowBar;

	GameObject shield;

private: 
	XBox::XBoxInput controller;
	XBox::Stick lStick;
	XBox::Stick rStick;
	int playerNum;
	float delay = 0.25f;
	float localTime = 0.0f;
	bool isTransformed = false;
	bool alive = true;

	//Vector containing all the projectiles the player has fired. Stored in player for collision purposes. 
	std::vector<Projectile*> projectiles;
};