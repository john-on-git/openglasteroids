#pragma once

#include <glad/glad.h>
#include <assimp/mesh.h>
#include <glm/ext/vector_float4.hpp>

/// <summary>
/// aiMesh, some stuff copied to buffer
/// mesh must be triangulated
/// </summary>
class BufferedAiMesh
{
	public:
		glm::vec4 colorMask;
		BufferedAiMesh();
		BufferedAiMesh(aiMesh* mesh, GLuint texture, glm::vec4 colorMask, GLuint textureLocation, GLuint colorLocation);

		~BufferedAiMesh();

		void Draw();
	protected:
		GLuint* VAO;
		GLuint* buffers;
		GLuint textureLocation;
		GLuint colorLocation;
		GLuint texture;
		GLsizei numIndices;
};
