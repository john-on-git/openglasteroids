#include <windows.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "myModel.h"


MyModel::MyModel()
{
	this->nVerts = -1;
	VBOs = new GLuint[1];
}
MyModel::MyModel(float verts[], unsigned int lenVertsIn, GLint VAO)
{
	//init properties
		VBOs = new GLuint[1];
		this->nVerts = lenVertsIn; //duration

		//generate, bind, copy verts to my buffer
			glad_glBindVertexArray(VAO);
			glad_glGenBuffers(1, VBOs); //19.5.21, first argument is the number of buffers, not the size of the buffer. corrupted the heap?
			glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
			glad_glBufferData( //add verts to buffer
				GL_ARRAY_BUFFER,
				nVerts * sizeof(float),
				verts,
				GL_STATIC_DRAW
			);
}
MyModel::MyModel(std::string path, GLint VAO)
{
	OutputDebugStringW(L"FATAL: called not-implemented constructor MyModel(std::string path)\n");
	exit(1);
	//TODO load the model
}

void MyModel::Draw()
{
	//draw
	glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glad_glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glad_glEnableVertexAttribArray(0);

	float* bufferData = new float[nVerts];
	glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glad_glGetBufferSubData(GL_ARRAY_BUFFER, 0, nVerts * sizeof(float), bufferData);

	glad_glDrawArrays(GL_POINTS, 0, nVerts);
}