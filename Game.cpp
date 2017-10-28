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
	circleEnemy.mesh.unload();
	circleEnemy.projectile.mesh.unload();
	player.blackBar.mesh.unload();
	player.yellowBar.mesh.unload();
	player2.shield.mesh.unload();

	player.tex.unload();
	player2.tex.unload();
	player.projectile.tex.unload();
	player2.projectile.tex.unload();
	basicEnemy.tex.unload();
	basicEnemy.projectile.tex.unload();
	circleEnemy.tex.unload();
	circleEnemy.projectile.tex.unload();
	player.blackBar.tex.unload();
	player.yellowBar.tex.unload();
	player2.shield.tex.unload();
	red.tex.unload();
	blue.tex.unload();
	purple.tex.unload();

	for (int i = 0; i < players.size(); i++)
	{
		players.erase(players.begin() + i);
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.erase(enemies.begin() + i);
	}
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
	light1.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	light1.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light1.specular = glm::vec3(0.1f, 0.1f, 0.1f);
	light1.specularExponent = 1.0f;
	light1.constantAttenuation = 0.5f;
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

	if (!circleEnemy.mesh.loadFromFile("meshes/sphere.obj"))
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

	if (!circleEnemy.projectile.mesh.loadFromFile("meshes/bullet1.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player.blackBar.mesh.loadFromFile("meshes/bar.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player.yellowBar.mesh.loadFromFile("meshes/bar.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	if (!player2.shield.mesh.loadFromFile("meshes/shield.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	player2.blackBar = player.blackBar;
	player2.yellowBar = player.yellowBar;

	cameraTransform = glm::translate(cameraTransform, glm::vec3(0.0f, 0.0f, -30.0f));
	cameraProjection = glm::perspective(45.0f, ((float)GetSystemMetrics(SM_CXSCREEN) / (float)GetSystemMetrics(SM_CYSCREEN)), 0.1f, 10000.0f);
	
	player.loadTexture("Textures/red.png");
	player.projectile.loadTexture("Textures/red.png");

	player2.loadTexture("Textures/blue.png");
	player2.projectile.loadTexture("Textures/blue.png");

	player.setNum(0);
	player2.setNum(1);

	basicEnemy.projectile.loadTexture("Textures/fur.png");
	basicEnemy.loadTexture("Textures/fur.png");

	circleEnemy.projectile.loadTexture("Textures/fur.png");
	circleEnemy.loadTexture("Textures/fur.png");

	player.blackBar.loadTexture("Textures/Black.png");
	player.yellowBar.loadTexture("Textures/Yellow.png");

	player.yellowBar.rotate = glm::rotate(player.yellowBar.rotate, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	player.blackBar.rotate = glm::rotate(player.blackBar.rotate, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	player.yellowBar.move(0.0f, -12.0f);
	player.blackBar.move(0.0f, -12.0f);

	player2.shield.loadTexture("Textures/shield.png");
	
	player.move(10.0f, -5.0f);
	player2.move(-10.0f, -5.0f);
	players.push_back(&player);
	players.push_back(&player2);

	

	std::cout << player.radius << std::endl;
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

		for (int i = 0; i < players.size(); i++)
		{
			if (players[i]->isAlive())
				players[i]->update(&enemies, players[(i + 1) % 2]);
		}

		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i]->update(players);

			if (enemies[i]->location.y <= -20)
			{
				enemies.erase(enemies.begin() + i);
				break;
			}
		}	

		if ((!player.isAlive()) && (!player2.isAlive()))
			state == gameOver;

		
		if (basicDelay >= BasicEnemy().spawnDelay)
		{
			std::cout << "basic delay" << std::endl;
			basicDelay = 0.0f;

			BasicEnemy* temp = new BasicEnemy();
			temp->radius = 0.3f;
			temp->mesh = basicEnemy.mesh;
			temp->tex = basicEnemy.tex;
			temp->projectile.mesh = basicEnemy.projectile.mesh;
			temp->projectile.tex = basicEnemy.projectile.tex;

			float x = (rand() % 20);
			temp->move(x, 20.0f);
			enemies.push_back(temp);
			std::cout << temp->radius << std::endl;

			if(basicEnemy.spawnDelay >= 1.0f)
				basicEnemy.spawnDelay /= 1.15f;
		}

		if (circleDelay >= CircleEnemy().spawnDelay)
		{
			std::cout << "circle delay" << std::endl;
			circleDelay = 0.0f;

			CircleEnemy* temp = new CircleEnemy();
			temp->radius = 0.3f;
			temp->mesh = circleEnemy.mesh;
			temp->tex = circleEnemy.tex;
			temp->projectile.mesh = circleEnemy.projectile.mesh;
			temp->projectile.tex = circleEnemy.projectile.tex;

			float x = (rand() % 20);
			temp->move(x, 20.0f);
			enemies.push_back(temp);

			if(circleEnemy.spawnDelay >= 2.0f)
				circleEnemy.spawnDelay /= 1.25f;
		}
	}

	if (state == gameOver)
	{
		delete updateTimer;

		phong.unload();
		player.mesh.unload();
		player2.mesh.unload();
		player.projectile.mesh.unload();
		player2.projectile.mesh.unload();
		basicEnemy.mesh.unload();
		basicEnemy.projectile.mesh.unload();
		circleEnemy.mesh.unload();
		circleEnemy.projectile.mesh.unload();
		player.blackBar.mesh.unload();
		player.yellowBar.mesh.unload();
		player2.shield.mesh.unload();

		player.tex.unload();
		player2.tex.unload();
		player.projectile.tex.unload();
		player2.projectile.tex.unload();
		basicEnemy.tex.unload();
		basicEnemy.projectile.tex.unload();
		circleEnemy.tex.unload();
		circleEnemy.projectile.tex.unload();
		player.blackBar.tex.unload();
		player.yellowBar.tex.unload();
		player2.shield.tex.unload();

		for (int i = 0; i < players.size(); i++)
		{
			players.erase(players.begin() + i);
		}

		for (int i = 0; i < enemies.size(); i++)
		{
			enemies.erase(enemies.begin() + i);
		}
	}
}

void Game::draw()
{
	if(state != gameOver)
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	else
		glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//phong.bind();

	//Iterate through vector of enemies and draw each one
	for (int i = 0; i < enemies.size(); i++)
	{
		enemies[i]->draw(phong, cameraTransform, cameraProjection, pointLights);

		for (int j = 0; j < enemies[i]->getProjectiles().size(); j++)
		{
			enemies[i]->getProjectiles()[j]->draw(phong, cameraTransform, cameraProjection, pointLights);
		}
	}

	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->isAlive())
		{
			players[i]->draw(phong, cameraTransform, cameraProjection, pointLights);

			for (int j = 0; j < players[i]->getProjectiles().size(); j++)
			{
				players[i]->getProjectiles()[j]->draw(phong, cameraTransform, cameraProjection, pointLights);
			}
		}
	}

	player.blackBar.draw(phong, cameraTransform, cameraProjection, pointLights);
	player.yellowBar.draw(phong, cameraTransform, cameraProjection, pointLights);
	
	//phong.unbind();

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