#include <windows.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "myModel.h"

unsigned long int MyModel::totalVertices = 0; //counter for total required for buffer

MyModel::MyModel()
{
	this->nVertices = -1;
	this->startVertices = -1;
	verts = NULL;
}
MyModel::MyModel(float vertsIn[], unsigned int nVerts)
{
	this->startVertices = totalVertices; //set start
	this->nVertices = nVerts; //duration
	this->totalVertices += nVerts; //update total

	verts = new float[nVerts][4]; //copy verts over to this model
	for (unsigned int i = 0, ii = 0;i < nVerts;i++, ii += 4)
		verts[i][0] = vertsIn[ii],
		verts[i][1] = vertsIn[ii + 1],
		verts[i][2] = vertsIn[ii + 2],
		verts[i][3] = vertsIn[ii + 3];
}
MyModel::MyModel(std::string path)
{
	OutputDebugStringW(L"FATAL: called not-implemented constructor MyModel(std::string path)\n");
	exit(1);
	//TODO load the model
}

/// <summary>
/// move this model's verts to the VBO
/// </summary>
void MyModel::MoveToBuffer()
{
	glad_glBufferSubData(GL_ARRAY_BUFFER, this->startVertices, this->nVertices * sizeof(float[4]), this->verts); //add verts to buffer

	free(verts);
	verts = NULL;
}