#pragma once

#include <string>
#include "../BufferedAiMesh/BufferedAiMesh.hpp"
#include <glm/ext/vector_float3.hpp>

class Model
{
	public:
		BufferedAiMesh* meshes;
		glm::vec3* bBox; //the bounding box
		Model(std::string path, GLuint textureLocation, GLuint colorLocation, GLuint* textures, glm::vec4* colorMasks, size_t numTextures);
		void Draw();
	protected:
		size_t numMeshes;
};