#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/functions.hpp>

Game::Game()
{

}

Game::~Game()
{
	delete updateTimer;
	
	phong.unload();
	player.mesh.unload();
	player2.mesh.unload();
	player.projectile.mesh.unload();
	player2.projectile.mesh.unload();
	basicEnemy.mesh.unload();
	basicEnemy.projectile.mesh.unload();

	player.tex.unload();
	player2.tex.unload();
	player.projectile.tex.unload();
	player2.projectile.tex.unload();
	basicEnemy.tex.unload();
	basicEnemy.projectile.tex.unload();
}

//Happens once at the beginning of the game
void Game::initializeGame()
{
	state = main;
	updateTimer = new Timer();
	std::srand(time(NULL));

	glEnable(GL_DEPTH_TEST);

	Light light1;

	light1.position = glm::vec4(0.0f, 0.0f, 15.0f, 1.0f);
	light1.ogPosition = light1.position;
	light1.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	light1.diffuse = glm::vec3(5.0f, 5.0f, 5.0f);
	light1.specular = glm::vec3(0.1f, 0.1f, 0.1f);
	light1.specularExponent = 1.0f;
	light1.constantAttenuation = 1.0f;
	light1.linearAttenuation = 0.1f;
	light1.quadraticAttenuation = 0.01f;

	pointLights.push_back(light1);

	//Loading in meshes
	if (!phong.load("shaders/phong.vert", "shaders/phong.frag"))
	{
		std::cout << "Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player.mesh.loadFromFile("meshes/Player_Ship.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player2.mesh.loadFromFile("meshes/Player_Ship.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player.projectile.mesh.loadFromFile("meshes/bullet1.obj"))
	{
		std::cout << "Projectile model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player2.projectile.mesh.loadFromFile("meshes/bullet1.obj"))
	{
		std::cout << "Projectile model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!basicEnemy.mesh.loadFromFile("meshes/cube.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!basicEnemy.projectile.mesh.loadFromFile("meshes/bullet1.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	cameraTransform = glm::translate(cameraTransform, glm::vec3(0.0f, 0.0f, -30.0f));
	cameraProjection = glm::perspective(45.0f, ((float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN)), 0.1f, 10000.0f);

	player.scale = glm::scale(player.transform, glm::vec3(0.3f, 0.3f, 0.3f));

	player.loadTexture("Textures/fur.png");
	player.projectile.loadTexture("Textures/fur.png");

	player2.scale = glm::scale(player2.transform, glm::vec3(0.3f, 0.3f, 0.3f));
	player2.loadTexture("Textures/fur.png");
	player2.projectile.loadTexture("Textures/fur.png");

	player.setNum(0);
	player2.setNum(1);

	basicEnemy.projectile.loadTexture("Textures/fur.png");

	basicEnemy.loadTexture("Textures/fur.png");
	
	players.push_back(&player);
	players.push_back(&player2);

}

//Happens once per frame, used to update state of the game
void Game::update()
{

	if (state == main)
	{
		//std::cout << "Main" << std::endl;
		//Update timer so we have correct delta time since last update
		updateTimer->tick();
		basicDelay += updateTimer->getElapsedTimeS();
		circleDelay += updateTimer->getElapsedTimeS();


		//Get input from controller
		player.xin();
		player2.xin();

		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i]->update(players);

			if (enemies[i]->location.y <= -20)
			{
				enemies.erase(enemies.begin() + i);
				break;
			}

			for (int j = 0; j < enemies[i]->projectiles.size(); j++)
			{
				if (enemies[i]->projectiles[j]->collide(player) || enemies[i]->projectiles[j]->collide(player2))
				{
					state = gameOver;
					std::cout << "Game Over" << std::endl;
				}
			}
		}
		std::cout << basicDelay << std::endl;
		if (basicDelay >= BasicEnemy().spawnDelay)
		{
			std::cout << "basic delay" << std::endl;
			basicDelay = 0.0f;

			BasicEnemy* temp = new BasicEnemy();
			temp->mesh = basicEnemy.mesh;
			temp->tex = basicEnemy.tex;
			temp->projectile.mesh = basicEnemy.projectile.mesh;
			temp->projectile.tex = basicEnemy.projectile.tex;

			float x = (rand() % 20);
			temp->move(x, 30.0f);
			enemies.push_back(temp);
		}

		for (int i = 0; i < players.size(); i++)
		{
			players[i]->update(enemies);
		}
	}

	if (state == gameOver)
	{
		phong.unload();
		player.mesh.unload();
		player2.mesh.unload();
		player.projectile.mesh.unload();
		player2.projectile.mesh.unload();
		basicEnemy.mesh.unload();
		basicEnemy.projectile.mesh.unload();

		player.tex.unload();
		player2.tex.unload();
		player.projectile.tex.unload();
		player2.projectile.tex.unload();
		basicEnemy.tex.unload();
		basicEnemy.projectile.tex.unload();
	}
}

void Game::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	phong.bind();

	player.draw(phong, cameraTransform, cameraProjection, pointLights);
	player2.draw(phong, cameraTransform, cameraProjection, pointLights);

	//Iterate through vector of enemies and draw each one
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->draw(phong, cameraTransform, cameraProjection, pointLights);

		for (int j = 0; j < enemies[i]->getProjectiles().size(); j++)
		{
			enemies[i]->getProjectiles()[j]->draw(phong, cameraTransform, cameraProjection, pointLights);
		}
	}

	//Iterate through vector of projectiles and draw each one
	for (int i = 0; i < player.getProjectiles().size(); i++)
	{
		player.getProjectiles()[i]->draw(phong, cameraTransform, cameraProjection, pointLights);
	}

	for (int i = 0; i < player2.getProjectiles().size(); i++)
	{
		player2.getProjectiles()[i]->draw(phong, cameraTransform, cameraProjection, pointLights);
	}
	
	phong.unbind();

	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	case 27:
	case 'q':
		exit(1);
		break;

	case 't':
		std::cout << "Total elapsed time: " << updateTimer->getCurrentTime() << std::endl;
		break;

	case 'r':
	case 'R':
		if (state == gameOver)
			initializeGame();

		break;

	case 'l':
	case 'L':
		shouldLightsSpin = !shouldLightsSpin;
		break;

	default:
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch (key)
	{
	default:
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			//handle left click
			break;

		case GLUT_RIGHT_BUTTON:
			//handle right click
			break;

		case GLUT_MIDDLE_BUTTON:
			//handle middle button
			break;

		default:
			break;
		}
	}
}

void Game::mouseMoved(int x, int y)
{

}