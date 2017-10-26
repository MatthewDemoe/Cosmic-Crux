#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Timer.h"
#include "ShaderProgram.h"
#include "EnemyManager.h"
#include <WinUser.h>

#define FRAMES_PER_SECOND 60

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();

	//Input Callback Functions
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	//Data Members
	Timer* updateTimer = nullptr;
	Player player;
	Player player2;

	//Vector of enemies, because we will be drawing some arbitrary amount
	std::vector<Enemy*> enemies;
	Enemy enemy;

	//Shaders
	ShaderProgram passThrough;
	ShaderProgram phongNoTexture;

	EnemyManager enemyManager;

	glm::mat4 cameraTransform;
	glm::mat4 cameraProjection;

private:
	
};