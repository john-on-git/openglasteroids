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
		BufferedAiMesh(aiMesh* mesh, GLuint texture, GLuint textureLocation, GLuint colorLocation);
		void Draw(glm::vec4 colorMask);
	protected:
		GLuint VAO;
		GLuint buffers[2]; //vertex buffer, and vertex index buffer
		GLuint textureLocation;
		GLuint colorLocation;
		GLuint textureHandle;
		GLsizei numIndices;
};
