#include "Joint.h"

Joint::Joint(int id, const std::string & name, const glm::mat4 & orginalBindTransform)
{
	m_ID = id;
	m_Name = name;
	m_OriginalBindTransform = orginalBindTransform;
	m_InverseBindTransform = glm::mat4(1.0f);
	m_Transformation = glm::mat4(1.0f);

}

Joint::Joint()
{
}

Joint::~Joint()
{
	destory();
}

void Joint::addChildJoint(Joint * joint)
{
	m_Childern.push_back(joint);
}

void Joint::destory()
{
	auto iter = m_Childern.begin();
	while (iter != m_Childern.end())
	{
		if ((*iter))
		{
			delete (*iter);
			(*iter) = nullptr;
			iter = m_Childern.erase(iter);
		}
	}
}

void Joint::calculateInverseBindTransform(glm::mat4 & parentBindTransform)
{
	glm::mat4 bindTransform = parentBindTransform*m_OriginalBindTransform;
	m_InverseBindTransform = glm::inverse(bindTransform);
	for (Joint * child : m_Childern)
	{
		child->calculateInverseBindTransform(bindTransform);
	}
}

void Joint::displayToConsole()
{
	printf("Joint %d - %s\n", m_ID, m_Name.c_str());
	for (Joint * child : m_Childern)
	{
		child->displayToConsole();
	}
}