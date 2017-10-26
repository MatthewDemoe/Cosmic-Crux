#pragma once
#include "Mesh.h"

class GameObject
{
public:
	//GameObject();

	//Constructor with defalt values
	GameObject(glm::vec2 loc = glm::vec2(0.0f, 0.0f), float rad = 0.15f);
	~GameObject();

	void move(float x, float y);
	void SetLocation(float, float);


	//Will hold the object's location. vec2 because z axis doesn't matter
	glm::vec2 location = glm::vec2(0, 0);
	float radius;

	bool collide(GameObject other);

	//Transformation matrix
	glm::mat4 transform;

	//
	glm::mat4 translate;
	glm::mat4 rotate;
	glm::mat4 scale;

	Mesh mesh;
private:
	
};