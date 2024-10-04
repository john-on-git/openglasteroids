#pragma once

#include <string>
#include "../BufferedAiMesh/BufferedAiMesh.hpp"
#include <glm/vec4.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat3x3.hpp>
#include <vector>

class Model
{
	public:
		BufferedAiMesh** meshes;
		glm::vec4 boundingMin;
		glm::vec4 boundingMax;
		size_t nEdges;
		glm::mat2x3* edges;
		size_t nFaces;
		glm::mat3x3* faces; //three points on face
		Model(std::string path, GLuint textureLocation, GLuint colorLocation, std::vector<GLuint> textures, size_t numTextures);
		~Model();
		void Draw(glm::vec4 colorMask);
	protected:
		size_t numMeshes;
};