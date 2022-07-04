#include <windows.h>

#include "BufferedAiMesh.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

BufferedAiMesh::BufferedAiMesh()
{
	OutputDebugStringW(L"FATAL: called not-implemented constructor MyTexturedModel()\n");
	exit(1);
}

constexpr auto STRIDE = 6; //width of each vert
constexpr auto NUM_POINTS_IN_A_TRIANGLE = 3;

BufferedAiMesh::BufferedAiMesh(std::string fileName, GLuint texture, GLuint textureLocation, GLuint VAO)
{
	Assimp::Importer importer;
	aiMesh* mesh = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices)->mMeshes[0];

	//init properties
		buffers = new GLuint[2]; //vertex buffer, and vertex index buffer
		this->texture = texture;
		this->textureLocation = textureLocation;
		this->numIndices = static_cast<size_t>(mesh->mNumFaces) * NUM_POINTS_IN_A_TRIANGLE;
	//transform verts to gl format
		float *verts = new float[mesh->mNumVertices * STRIDE];
		for (size_t i = 0;i < mesh->mNumVertices;i++)
		{
			size_t vertsPos = i * STRIDE;
			//position
			verts[vertsPos]		= mesh->mVertices[i].x;
			verts[vertsPos + 1] = mesh->mVertices[i].y;
			verts[vertsPos + 2] = mesh->mVertices[i].z;
			verts[vertsPos + 3] = 1.0f;
			//texture
			verts[vertsPos + 4] = mesh->mTextureCoords[0][i].x;
			verts[vertsPos + 5] = mesh->mTextureCoords[0][i].y;
		}

		unsigned int *vertIndices = new unsigned int[mesh->mNumFaces * NUM_POINTS_IN_A_TRIANGLE];
		for (size_t i = 0;i < static_cast<size_t>(mesh->mNumFaces);i++)
			vertIndices[i * NUM_POINTS_IN_A_TRIANGLE + 0] = (mesh->mFaces[i].mIndices)[0],
			vertIndices[i * NUM_POINTS_IN_A_TRIANGLE + 1] = (mesh->mFaces[i].mIndices)[1],
			vertIndices[i * NUM_POINTS_IN_A_TRIANGLE + 2] = (mesh->mFaces[i].mIndices)[2];
		
	//generate, bind, copy verts to my buffer
		glad_glBindVertexArray(VAO);
		glad_glGenBuffers(2, buffers); //19.5.21, first argument is the number of buffers, not the size of the buffer. corrupted the heap?
		
		glad_glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glad_glBufferData( //add verts to buffer
			GL_ARRAY_BUFFER,
			static_cast<size_t>(mesh->mNumVertices) * STRIDE * sizeof(float),
			verts,
			GL_STATIC_DRAW
		);

		glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glad_glBufferData( //add verts to buffer
			GL_ELEMENT_ARRAY_BUFFER,
			static_cast<size_t>(mesh->mNumFaces) * NUM_POINTS_IN_A_TRIANGLE * sizeof(unsigned int),
			vertIndices,
			GL_STATIC_DRAW
		);

		glad_glVertexAttribPointer( //position
			0,
			4,
			GL_FLOAT,
			GL_FALSE,
			STRIDE * sizeof(float), 
			/*	1.6.22 
				Value passed here was incorrect, forgot to add the sizeof() here after removing it from STRIDE definition.
				Resulted in all sorts of bizarre rendering errors, and I was convinced that the problem was with the mesh data.
			*/
			NULL
		);
		glad_glEnableVertexAttribArray(0);
		glad_glVertexAttribPointer( //texCoord
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			STRIDE * sizeof(float), //1.6.22 same here
			(GLvoid*)(4 * sizeof(float)) //offset
		);
		glad_glEnableVertexAttribArray(1);

		delete[] verts;
		delete[] vertIndices;
	}

void BufferedAiMesh::Draw()
{
	//bind the buffer
		glad_glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	//draw every polygon
		//set uniform value
		glad_glActiveTexture(GL_TEXTURE0);
		glad_glBindTexture(GL_TEXTURE_2D, texture);
		glad_glUniform1i(textureLocation, 0);
		//draw
		glad_glDrawElements(
			GL_TRIANGLES,
			numIndices,
			GL_UNSIGNED_INT,
			NULL
		);
}

BufferedAiMesh::~BufferedAiMesh() {
	delete buffers;
}