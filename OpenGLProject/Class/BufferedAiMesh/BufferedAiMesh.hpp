#pragma once

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
		BufferedAiMesh(aiMesh* mesh, GLuint texture, GLuint textureLocation);

		~BufferedAiMesh();

		void Draw();
	protected:
		GLuint* VAO;
		GLuint* buffers;
		GLuint textureLocation;
		GLuint texture;
		GLsizei numIndices;
};
