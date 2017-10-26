#include "EnemyManager.h"

EnemyManager::EnemyManager()
{

}

EnemyManager::EnemyManager(float time)
{
	spawnTime = time;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Spawn(float dt)
{
	if (timer >= spawnTime)
	{
		timer = 0;

		for (int i = 0; i < enemyList.size(); i++)
		{
			enemyList[i]->SetLocation(rand() % -10, 6);
			std::cout << "Setting location" << std::endl;
		}
	}
	else
	{
		timer += dt;

		
	}

	

}

void EnemyManager::Update(float dt)
{
	Spawn(dt);
		enemy.update(dt);
	for (int i = 0; i < enemyList.size(); i++)
	{
		enemyList[i]->update(dt);

	}
}

void EnemyManager::Draw(ShaderProgram &shader)
{
	for (int i = 0; i < enemyList.size(); i++)
	{
		shader.sendUniformMat4("uModel", glm::value_ptr(enemyList[i]->transform), false);

		glBindVertexArray(enemyList[i]->mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, enemyList[i]->mesh.getNumVertices());
		glBindVertexArray(GL_NONE);
		enemyList[i]->drawProjectiles(shader);
	}
}

void EnemyManager::Intialize()
{
	if (!enemy.mesh.loadFromFile("meshes/cube.obj"))
	{
		std::cout << "Player model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	enemy.scale = glm::scale(enemy.scale, glm::vec3(0.3f, 0.3f, 0.3f));
	enemy.move(0, 7);
	enemyList.push_back(&enemy);
}


