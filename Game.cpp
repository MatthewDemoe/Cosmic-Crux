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
	
	passThrough.unload();
	phongNoTexture.unload();
	player.mesh.unload();
}

//Happens once at the beginning of the game
void Game::initializeGame()
{
	updateTimer = new Timer();

	glEnable(GL_DEPTH_TEST);

	enemyManager = EnemyManager(2.5f);

	//Loading in meshes 
	if (!passThrough.load("shaders/passThrough.vert", "shaders/passthrough.frag"))
	{
		std::cout << "Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!phongNoTexture.load("shaders/phong.vert", "shaders/phongNoTexture.frag"))
	{
		std::cout << "Shaders failed to initialize." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player.mesh.loadFromFile("meshes/monkey.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player2.mesh.loadFromFile("meshes/monkey.obj"))
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

	if (!enemy.mesh.loadFromFile("meshes/cube.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}
	
	//Making starting enemy
	enemyManager.Intialize();


	cameraTransform = glm::translate(cameraTransform, glm::vec3(0.0f, 0.0f, -10.0f));
	cameraProjection = glm::perspective(45.0f, ((float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN)), 0.1f, 10000.0f);
	player.scale = glm::scale(player.transform, glm::vec3(0.3f, 0.3f, 0.3f));
}

//Happens once per frame, used to update state of the game
void Game::update()
{

	std::srand(time(NULL));

	//Update timer so we have correct delta time since last update
	updateTimer->tick();

	//Sets the player number, used for controller input (Player 1 uses controller 0, player 2 uses controller 1). I wanted a cleaner way to do this, but it works for now. 
	player.setNum(0);
	//player2.setNum(1);

	//Get input from controller
	player.xin();
	//player2.xin();

	float deltaTime = updateTimer->getElapsedTimeS();

	//enemyManager.Spawn(deltaTime);

	enemyManager.Update(deltaTime);

	//Iterate through projectile vector and move them all in the direction of their velocity
	for (int i = 0; i < player.getProjectiles().size(); i++)
	{
		player.getProjectiles()[i]->move(player.getProjectiles()[i]->getVelocity().x, player.getProjectiles()[i]->getVelocity().y);

		//Erase the projectile if it leaves the screen 
		if ((player.getProjectiles()[i]->location.x >= 5) || (player.getProjectiles()[i]->location.y >= 5) 
			|| (player.getProjectiles()[i]->location.x <= -5) || (player.getProjectiles()[i]->location.y <= -5))
		{
			player.deleteProjectile(i);
			break;
		}

		//Iterate through each enemy, check if current projectile is intersecting with it
		for (int j = 0; j < enemies.size(); j++)
		{
			if (player.getProjectiles()[i]->collide(*enemies[j]))
			{
				//Erase projectile, Erase enemy and spawn in a new location for now. 
				player.deleteProjectile(i);
				//enemies.erase(enemies.begin() + j);

				float x = (rand() % 10) - 5;
				float y = (rand() % 10) - 5;
				
				//enemy.move(-enemy.location.x, -enemy.location.y);
				//enemy.move(x, y);
				//enemies.push_back(&enemy);
			}
		}
	}
}

void Game::draw()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render stuff
	//passThrough.bind();
	//passThrough.sendUniformMat4("uModel", glm::value_ptr(player.transform), false);
	//passThrough.sendUniformMat4("uView", glm::value_ptr(cameraTransform), false);
	//passThrough.sendUniformMat4("uProj", glm::value_ptr(cameraProjection), false);

	phongNoTexture.bind();
	phongNoTexture.sendUniformMat4("uModel", glm::value_ptr(player.transform), false);
	phongNoTexture.sendUniformMat4("uView", glm::value_ptr(cameraTransform), false);
	phongNoTexture.sendUniformMat4("uProj", glm::value_ptr(cameraProjection), false);

	phongNoTexture.sendUniform("lightPos", cameraTransform * glm::vec4(4.0f, 0.0f, 0.0f, 1.0f));
	phongNoTexture.sendUniform("objectColor", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	phongNoTexture.sendUniform("lightAmbient", glm::vec3(0.5f, 0.5f, 0.5f));
	phongNoTexture.sendUniform("lightDiffuse", glm::vec3(5.0f, 5.0f, 5.0f));
	phongNoTexture.sendUniform("lightSpecular", glm::vec3(5.0f, 5.0f, 5.0f));

	phongNoTexture.sendUniform("lightSpecularExponent", 50.0f);
	phongNoTexture.sendUniform("attenuationConstant", 1.0f);
	phongNoTexture.sendUniform("attenuationLinear", 0.1f);
	phongNoTexture.sendUniform("attenuationQuadratic", 0.1f);

	glBindVertexArray(player.mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, player.mesh.getNumVertices());
	glBindVertexArray(GL_NONE);

	//passThrough.sendUniformMat4("uModel", glm::value_ptr(player2.transform), false);
	//
	//glBindVertexArray(player2.vao);
	//glDrawArrays(GL_TRIANGLES, 0, player2.getNumVertices());
	//glBindVertexArray(GL_NONE);

	//Iterate through vector of enemies and draw each one
	for (int i = 0; i < enemies.size(); i++)
	{
		phongNoTexture.sendUniformMat4("uModel", glm::value_ptr(enemies[i]->transform), false);

		glBindVertexArray(enemies[i]->mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, enemies[i]->mesh.getNumVertices());
		glBindVertexArray(GL_NONE);
	}

	//Iterate through vector of projectiles and draw each one
	for (int i = 0; i < player.getProjectiles().size(); i++)
	{
		phongNoTexture.sendUniformMat4("uModel", glm::value_ptr(player.getProjectiles()[i]->transform), false);

		glBindVertexArray(player.getProjectiles()[i]->mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, player.getProjectiles()[i]->mesh.getNumVertices());
		glBindVertexArray(GL_NONE);
	}
	
	enemyManager.Draw(phongNoTexture);


	phongNoTexture.unbind();

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
		//player.isRotating = !player.isRotating;
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