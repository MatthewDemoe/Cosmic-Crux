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

void GameObject::draw(ShaderProgram &shader, glm::mat4 cameraTransform, glm::mat4 cameraProjection, std::vector<Light> pointLights)
{
	tex.bind();
	shader.sendUniformMat4("uModel", glm::value_ptr(transform), false);
	shader.sendUniformMat4("uView", glm::value_ptr(cameraTransform), false);
	shader.sendUniformMat4("uProj", glm::value_ptr(cameraProjection), false);
		  
	shader.sendUniform("uTex", 0);

	for (int i = 0; i < pointLights.size(); i++)
	{
		std::string prefix = "pointLight.";
		shader.sendUniform(prefix + "position", cameraTransform * pointLights[i].position);
		shader.sendUniform(prefix + "ambient", pointLights[i].ambient);
		shader.sendUniform(prefix + "diffuse", pointLights[i].diffuse);
		shader.sendUniform(prefix + "specular", pointLights[i].specular);

		shader.sendUniform(prefix + "specularExponent", pointLights[i].specularExponent);
		shader.sendUniform(prefix + "constantAttenuation", pointLights[i].constantAttenuation);
		shader.sendUniform(prefix + "linearAttenuation", pointLights[i].linearAttenuation);
		shader.sendUniform(prefix + "quadraticAttenuation", pointLights[i].quadraticAttenuation);
	}

	//shader.sendUniform("objectColor", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));


	glBindVertexArray(mesh.vao);
	glDrawArrays(GL_TRIANGLES, 0, mesh.getNumVertices());
	glBindVertexArray(GL_NONE);
	tex.unbind();
}

void GameObject::loadTexture(const std::string & texFile)
{
	if (!tex.load(texFile))
	{
		system("pause");
		exit(0);
	}
}


