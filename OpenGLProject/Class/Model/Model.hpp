#pragma once

#include <string>
#include "../BufferedAiMesh/BufferedAiMesh.hpp"

class Model
{
	public:
		Model(std::string path, GLuint textureLocation, GLuint* textures, size_t numTextures);
		void Draw();
	protected:
		BufferedAiMesh* meshes;
		size_t numMeshes;
};