#pragma once
#include <vector>
#include "Enemy.h"
#include <iostream>
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
class EnemyManager
{
public:
	EnemyManager();
	EnemyManager(float time);
	~EnemyManager();
	void Spawn(float dt);
	void Intialize();
	void Update(float);
	void Draw(ShaderProgram & shader);
	void Draw();
	std::vector<Enemy*> enemyList;
	Enemy enemy;

private:
	int numberOfEnemies;
	float spawnTime;
	float timer = 0;

};