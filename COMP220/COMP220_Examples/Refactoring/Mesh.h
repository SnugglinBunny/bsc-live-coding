#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>

#include <vector>
#include "vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void copyMeshData(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	void init();
	void destroy();

	void render();

private:
	GLuint m_VBO;
	GLuint m_EBO;

	unsigned int m_numberOfVerts;
	unsigned int m_numberOfIndices;

	GLuint m_VAO;
};

