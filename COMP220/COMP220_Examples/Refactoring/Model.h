#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <GL\glew.h>
#include <SDL_opengl.h>

#include <string>
#include <vector>

#include "vertex.h"
#include "Mesh.h"

bool loadModelFromFile(const std::string& filename, GLuint VBO, GLuint EBO, unsigned int& numberOfVerts, unsigned int& numberOfIndices);

bool loadMeshesFromFile(const std::string& filename, std::vector<Mesh*>& meshes);