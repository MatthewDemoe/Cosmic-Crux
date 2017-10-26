#include <iostream>
#include "GameObject.h"

//GameObject::GameObject()
//{
//	location = glm::vec2(0.0f, 0.0f);
//}

//Location default is (0, 0), radius default is 0.15
GameObject::GameObject(glm::vec2 loc, float rad) :  radius(rad)
{
	move(loc.x, loc.y);
}

GameObject::~GameObject() 
{

}

//Change the object's location and the object mesh location
void GameObject::move(float x, float y)
{
	location.x += x;
	location.y += y;

	translate = glm::translate(translate, glm::vec3(x, y, 0.0f));
	transform = glm::translate(translate, glm::vec3(0.0f, 0.0f, 0.0f)) * rotate * scale;
	//transform = translate  * rotate * scale;
}

//Check if two objects are intersecting, just using collisions spheres for now
bool GameObject::collide(GameObject other)
{
	glm::vec2 temp = glm::vec2(location.x - other.location.x, location.y - other.location.y);
	float mag = sqrt((temp.x * temp.x) + (temp.y * temp.y));

	if (mag < (radius + other.radius))
	{
		return true;
	}

	return false;
}

void GameObject::SetLocation(float x, float y)
{
	float resetX = -location.x;
	float resetY = -location.y;
	move(resetX, resetY);
	move(x, y);
}


