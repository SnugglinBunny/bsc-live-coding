#pragma once
#include <vector>
#include "Mesh.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void setPostition(const glm::vec3& position)
	{
		m_Position = position;
	};

	glm::vec3& getPosition()
	{
		return m_Position;
	};

	void setRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
	};

	glm::vec3& getRotation()
	{
		return m_Rotation;
	};

	void setScale(const glm::vec3& scale)
	{
		m_Scale = scale;
	};

	glm::vec3& getScale()
	{
		return m_Scale;
	};

	glm::mat4& getModelMatrix()
	{
		return m_ModelMatrix;
	};

	void setSpecularPower(float power)
	{
		m_SpecularPower = power;
	};

	float getSpecularPower()
	{
		return m_SpecularPower;
	};

	GLuint getShaderProgramID()
	{
		return m_ShaderProgramID;
	};

	void loadMeshFromFile(const std::string& filename);
	void loadDiffuseMap(const std::string& filename);
	void loadShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	void destroy();

	void update();

	void preRender();

	void render();
private:
	std::vector<Mesh*> m_Meshes;

	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_Rotation;

	glm::mat4 m_ModelMatrix;
	
	GLuint m_DiffuseMapID;
	glm::vec4 m_ambientMaterialColour;
	glm::vec4 m_diffuseMaterialColour;
	glm::vec4 m_specularMaterialColour;
	float m_SpecularPower;
	GLuint m_ShaderProgramID;
};