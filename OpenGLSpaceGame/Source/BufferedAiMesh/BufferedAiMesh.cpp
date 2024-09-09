#include "BufferedAiMesh.hpp"
#include "windows.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

BufferedAiMesh::BufferedAiMesh()
{
	VAO = 0;
	buffers = nullptr;
	textureLocation = 0;
	texture = 0;
	numIndices = 0;
}

constexpr auto STRIDE = 6; //width of each vert
constexpr auto INDICES_PER_TRI = 3;

BufferedAiMesh::BufferedAiMesh(aiMesh* mesh, GLuint texture, glm::vec4 colorMask, GLuint textureLocation, GLuint colorLocation)
{
	//init properties
		this->texture = texture;
		this->colorMask = colorMask;
		this->textureLocation = textureLocation;
		this->colorLocation = colorLocation;
		this->numIndices = (mesh->mNumFaces) * INDICES_PER_TRI;
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

		unsigned int *vertIndices = new unsigned int[mesh->mNumFaces * INDICES_PER_TRI];
		for (size_t i = 0;i < static_cast<size_t>(mesh->mNumFaces);i++)
			vertIndices[i * INDICES_PER_TRI + 0] = (mesh->mFaces[i].mIndices)[0],
			vertIndices[i * INDICES_PER_TRI + 1] = (mesh->mFaces[i].mIndices)[1],
			vertIndices[i * INDICES_PER_TRI + 2] = (mesh->mFaces[i].mIndices)[2];
	//generate vertex array object
		VAO = new GLuint;
		glad_glGenVertexArrays(1, VAO);
		glad_glBindVertexArray(*VAO);
	//generate buffers and copy over data
		buffers = new GLuint[2]; //vertex buffer, and vertex index buffer
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
			static_cast<size_t>(mesh->mNumFaces) * INDICES_PER_TRI * sizeof(unsigned int),
			vertIndices,
			GL_STATIC_DRAW
		);

		delete[] verts;
		delete[] vertIndices;
	//set vertex attrib pointers
		glad_glEnableVertexAttribArray(0);
		glad_glVertexAttribPointer( //position
			0,
			4,
			GL_FLOAT,
			GL_FALSE,
			STRIDE * sizeof(float), 
			/*	1.7.22 
				Value passed here was incorrect, forgot to add the sizeof() here after removing it from STRIDE definition.
				Resulted in all sorts of bizarre rendering errors, and I was convinced that the problem was with the mesh data.
			*/
			nullptr
		);

		glad_glEnableVertexAttribArray(1);
		glad_glVertexAttribPointer( //texCoord
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			STRIDE * sizeof(float), //1.7.22 same here
			(GLvoid*)(4 * sizeof(float)) //offset
		);
	}

void BufferedAiMesh::Draw()
{
	//bind the buffer (no, don't! https://stackoverflow.com/a/27877433) 7.7.22
	//bind the array object instead
	glad_glBindVertexArray(*VAO);
	//draw every polygon
		//set uniform value
		glad_glActiveTexture(GL_TEXTURE0);

		glad_glBindTexture(GL_TEXTURE_2D, texture); 
		//apparently the correct way to do this is to build a RAM representation of which textures are bound, and first use it to check if the texture is already bound, 
		//as it's cheaper than redundantly sending it to the GPU (a very expensive operation at this timescale)
		
		glad_glUniform1i(textureLocation, 0);
		glad_glUniform4fv(colorLocation, 1, glm::value_ptr(colorMask));
		//draw
		glad_glDrawElements(
			GL_TRIANGLES,
			numIndices,
			GL_UNSIGNED_INT,
			nullptr
		);
}

BufferedAiMesh::~BufferedAiMesh() {
	delete buffers;
}