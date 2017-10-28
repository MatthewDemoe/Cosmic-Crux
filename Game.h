#pragma once
#include "Player.h"
#include "BasicEnemy.h"
#include "CircleEnemy.h"
#include "Light.h"
#include "ShaderProgram.h"
#include <WinUser.h>
#include <vector>

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
	std::vector<Player*> players;

	//Vector of enemies, because we will be drawing some arbitrary amount
	BasicEnemy basicEnemy;
	CircleEnemy circleEnemy;
	std::vector<Enemy*> enemies;
	std::vector<Light> pointLights;
	GameObject red; 
	GameObject blue; 
	GameObject purple;

	//Shaders
	ShaderProgram phong;

	glm::mat4 cameraTransform;
	glm::mat4 cameraProjection;
	glm::mat4 lightSpinner;

private:
	bool shouldLightsSpin = false;
	enum gameStates{main, gameOver};
	gameStates state;
	float basicDelay = 0.0f;
	float circleDelay = 0.0f;
};