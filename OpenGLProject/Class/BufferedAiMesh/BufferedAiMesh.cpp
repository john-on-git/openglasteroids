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
			//verts are being copied over incorrectly.
			//position
			verts[i * STRIDE + 0] = mesh->mVertices[i].x;
			verts[i * STRIDE + 1] = mesh->mVertices[i].y;
			verts[i * STRIDE + 2] = mesh->mVertices[i].z;
			verts[i * STRIDE + 3] = 1.0f;
			//texture
			verts[i * STRIDE + 4] = 0.0f;
			verts[i * STRIDE + 5] = 0.0f;
			//verts[i + 4] = mesh->mTextureCoords[0][i].x;
			//verts[i + 5] = mesh->mTextureCoords[0][i].y;
		}

		unsigned int *vertIndices = new unsigned int[mesh->mNumFaces * NUM_POINTS_IN_A_TRIANGLE];
		size_t i = 0;
		for (size_t j = 0;j < static_cast<size_t>(mesh->mNumFaces);j++)
			for (char k = 0; k < NUM_POINTS_IN_A_TRIANGLE; k++)
				vertIndices[i++] = (mesh->mFaces[j].mIndices)[k];
		
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

		glad_glVertexAttribPointer( //position
			0,
			4,
			GL_FLOAT,
			GL_FALSE,
			STRIDE,
			NULL
		);
		glad_glVertexAttribPointer( //texCoord
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			STRIDE,
			(GLvoid*)(4 * sizeof(float)) //offset
		);
		
		glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glad_glBufferData( //add verts to buffer
			GL_ELEMENT_ARRAY_BUFFER,
			static_cast<size_t>(mesh->mNumFaces) * NUM_POINTS_IN_A_TRIANGLE * sizeof(unsigned int),
			vertIndices,
			GL_STATIC_DRAW
		);

		//print all verts
		//std::cout << "verts: " << std::endl;
		//for (size_t i = 0;i < static_cast<size_t>(mesh->mNumVertices);i++)
		//	std::cout						<<
		//	'\t' << i << ": ("				<<
		//	verts[i * STRIDE + 0] << ", "	<<
		//	verts[i * STRIDE + 1] << ", "	<<
		//	verts[i * STRIDE + 2] << ", "	<<
		//	verts[i * STRIDE + 3] << ")"	<<
		//	std::endl;
		//std::cout << std::endl;
		//
		////print all faces
		//std::cout << "faces: " << std::endl;
		//for (size_t i = 0;i < static_cast<size_t>(mesh->mNumFaces);i++)
		//{
		//	std::cout					<<
		//	'\t' <<"face "	<< i << ":"	<<
		//	std::endl << "\t\t"			<<
		//	vertIndices[i + 0] << ' '	<<
		//	vertIndices[i + 1] << ' '	<<
		//	vertIndices[i + 2] << ' '	<<
		//	std::endl;
		//}
		//std::cout << std::endl;

		////print all indices
		//std::cout << "indices: " << std::endl;
		//for (size_t i = 0;i < mesh->mNumFaces * NUM_POINTS_IN_A_TRIANGLE;i++)
		//	std::cout							<<
		//	'\t' << i << ": "					<<
		//	vertIndices[i] << " ("				<<
		//	verts[vertIndices[i] + 0] << ", "	<<
		//	verts[vertIndices[i] + 1] << ", "	<<
		//	verts[vertIndices[i] + 2] << ", "	<<
		//	verts[vertIndices[i] + 3] << ")"	<<
		//	std::endl;

		delete[] verts;
		delete[] vertIndices;
}

void BufferedAiMesh::Draw()
{
	//bind the buffer
		glad_glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glad_glEnableVertexAttribArray(0);
		glad_glEnableVertexAttribArray(1);
	//draw every polygon
		//set uniform value
		glad_glActiveTexture(GL_TEXTURE0);
		glad_glBindTexture(GL_TEXTURE_2D, texture);
		glad_glUniform1i(textureLocation, 0);
		//draw
		glad_glDrawElements(
			GL_LINES,
			numIndices,
			GL_UNSIGNED_INT,
			NULL
		);
}

BufferedAiMesh::~BufferedAiMesh() {
	delete buffers;
}