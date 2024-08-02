#pragma once

#include <string>
#include "../BufferedAiMesh/BufferedAiMesh.hpp"
#include <glm/vec4.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat2x3.hpp>
#include <vector>

class Model
{
	public:
		BufferedAiMesh* meshes;
		glm::vec4 boundingMin;
		glm::vec4 boundingMax;
		std::vector<glm::mat2x4> edges;
		std::vector<glm::mat2x3> faces; //point on face, then normal
		Model(std::string path, GLuint textureLocation, GLuint colorLocation, std::vector<GLuint> textures, std::vector<glm::vec4> colorMasks, size_t numTextures);
		void Draw();
	protected:
		size_t numMeshes;
};