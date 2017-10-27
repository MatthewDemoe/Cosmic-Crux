#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Light.h"
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class GameObject
{
public:
	//GameObject();

	//Constructor with defalt values
	GameObject(glm::vec2 loc = glm::vec2(0.0f, 0.0f), float rad = 0.15f);
	~GameObject();

	void move(float x, float y);

	//Will hold the object's location. vec2 because z axis doesn't matter
	glm::vec2 location = glm::vec2(0.0f, 0.0f);

	bool collide(GameObject other);
	void draw(ShaderProgram &shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection, std::vector<Light> pointLights);
	void loadTexture(const std::string &texFile);

	//For collision sphere
	float radius;

	//Transformation matrix
	glm::mat4 transform;

	glm::mat4 translate;
	glm::mat4 rotate;
	glm::mat4 scale;

	Mesh mesh;
	Texture tex;

private:
	
};