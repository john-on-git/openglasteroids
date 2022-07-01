#pragma once

#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <assimp/mesh.h>

/// <summary>
/// aiMesh, some stuff copied to buffer
/// mesh must be triangulated
/// </summary>
class BufferedAiMesh
{
	public:
		BufferedAiMesh();
		BufferedAiMesh(std::string fileName, GLuint texture, GLuint textureLocation, GLuint VAO);

		~BufferedAiMesh();

		void Draw();
	protected:
		GLuint *buffers;
		GLuint textureLocation;
		GLuint texture;
		size_t numIndices;
};
