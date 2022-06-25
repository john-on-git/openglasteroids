#include <windows.h>

#include "MyTexturedModel.hpp"
#include <glm/gtc/type_ptr.hpp>

MyTexturedModel::MyTexturedModel()
{
	OutputDebugStringW(L"FATAL: called not-implemented constructor MyTexturedModel()\n");
	exit(1);
}

constexpr auto stride = 6 * sizeof(float); //width of each vert

MyTexturedModel::MyTexturedModel(size_t nPolygons, float *verts, GLuint *textures, unsigned char *polygonBoundaries, GLuint textureLocation, GLuint VAO)
{
	//init properties
		this->nPolygons = nPolygons;
		VBOs = new GLuint[1];
		this->polygonBoundaries = polygonBoundaries;
	//get total number of verts
		size_t nVerts = 0;
		for (size_t i = 0;i < nPolygons;i++)
			nVerts += polygonBoundaries[i];
	//generate, bind, copy verts to my buffer
		glad_glBindVertexArray(VAO);
		glad_glGenBuffers(1, VBOs); //19.5.21, first argument is the number of buffers, not the size of the buffer. corrupted the heap?
		glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glad_glBufferData( //add verts to buffer
			GL_ARRAY_BUFFER,
			nVerts * stride,
			verts,
			GL_STATIC_DRAW
		);

	this->textures = textures;
	this->textureLocation = textureLocation;
}

MyTexturedModel::MyTexturedModel(std::string, GLuint VAO)
{
	OutputDebugStringW(L"FATAL: called not-implemented constructor MyTexturedModel(std::string path)\n");
	exit(1);
}

void MyTexturedModel::Draw()
{
	//bind the buffer
		glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glad_glVertexAttribPointer( //position
			0,
			4,
			GL_FLOAT,
			GL_FALSE,
			stride,
			NULL
		);
		glad_glVertexAttribPointer( //texCoord
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			stride,
			(GLvoid*)(4 * sizeof(float)) //offset
		);
		glad_glEnableVertexAttribArray(0);
		glad_glEnableVertexAttribArray(1);
	//draw every polygon
		size_t start = 0;
		for (size_t i = 0;i < nPolygons;i++)
		{
			//set uniform value
				glad_glActiveTexture(GL_TEXTURE0);
				glad_glBindTexture(GL_TEXTURE_2D, textures[i]);
				glad_glUniform1i(textureLocation, 0);
			//draw
				glad_glDrawArrays(GL_TRIANGLE_STRIP, start, polygonBoundaries[i]);
				start += polygonBoundaries[i];
		}
}