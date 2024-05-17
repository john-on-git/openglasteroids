#pragma once

#include <string>
#include "../BufferedAiMesh/BufferedAiMesh.hpp"
#include <glm/ext/vector_float3.hpp>
#include <vector>

class Model
{
	public:
		BufferedAiMesh* meshes;
		std::vector<glm::vec4> verts;
		Model(std::string path, GLuint textureLocation, GLuint colorLocation, GLuint* textures, glm::vec4* colorMasks, size_t numTextures);
		void Draw();
	protected:
		size_t numMeshes;
};