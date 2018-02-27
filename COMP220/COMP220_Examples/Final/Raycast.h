#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "btBulletDynamicsCommon.h"

#include "Camera.h"

class Raycast //Creating raycast class
{
public:
	Raycast();
	~Raycast();

	void update(Camera* camera, btDynamicsWorld* dynamicsWorld); //Updates the raycast based on inputs

	void destroy();

private:
	//Raycast Start & End positions
	glm::vec4 rayStartPosition;
	glm::vec4 rayEndPosition;
};
