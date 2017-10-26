#include "Enemy.h"

Enemy::Enemy()
{
	if (!projectile.mesh.loadFromFile("meshes/bullet1.obj"))
	{
		//std::cout << "Projectile model failed to load." << std::endl;
		system("pause");
		exit(0);
	}

	fireRate = 0.5f;
}

Enemy::~Enemy()
{

}

void Enemy::update(float dt)
{
	move(0, -1 * dt);

	if (fireTimer >= fireRate)
	{
		shoot();
		fireTimer = 0;
	}
	else
		fireTimer += dt;




	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i]->move(0 * dt, -2 * dt);
	}
	//translate = glm::translate(translate, glm::vec3(0, -1 * dt, 0));
	//
	//transform = translate * rotate * scale;
}

void Enemy::shoot()
{
	//Projectile pointer to be pushed back into the vector of projectiles
	Projectile* temp = new Projectile();

	//Set the mesh of the new projectile
	temp->mesh = projectile.mesh;

	//Make the new projectile's starting location equal to the player's 
	temp->transform = transform;
	temp->location = location;
	temp->rotate = glm::rotate(temp->transform, -90.0f, glm::vec3(1, 0, 0));

	//Assign velovity
	//temp->velocity = glm::vec2(0.1 * normalVel.x, 0.1 * normalVel.y);
	temp->SetVelocity(0, 1);

	projectiles.push_back(temp);
}

void Enemy::drawProjectiles(ShaderProgram &shader)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		shader.sendUniformMat4("uModel", glm::value_ptr(projectiles[i]->transform), false);

		glBindVertexArray(projectiles[i]->mesh.vao);
		glDrawArrays(GL_TRIANGLES, 0, projectiles[i]->mesh.getNumVertices());
		glBindVertexArray(GL_NONE);
	}
}