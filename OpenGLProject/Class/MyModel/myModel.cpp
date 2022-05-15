#include <windows.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "myModel.h"

MyModel::MyModel()
{
	this->nVertices = -1;
	this->VBO = -1;
}
MyModel::MyModel(float verts[], unsigned int nVerts)
{
	float(*verts4D)[4] = new float[nVerts][4];
	this->nVertices = nVerts; 
	glad_glGenBuffers(1, &(VBO)); //instantiate my buffer

	for (int i=0, ii=0;i<nVerts;i++, ii+=2)
		verts4D[i][0] = verts[ii],
		verts4D[i][1] = verts[ii+1],
		verts4D[i][2] = 0.0f,
		verts4D[i][3] = 0.0f;
	
	BindBuffer(); 
	glad_glBufferData(GL_ARRAY_BUFFER, nVerts * sizeof(float[4]), verts4D, GL_STATIC_DRAW); //add verts to buffer
}
MyModel::MyModel(std::string path)
{
	OutputDebugStringW(L"FATAL: called not-implemented constructor MyModel(std::string path)\n");
	exit(1);
	//TODO load the model
}

void MyModel::BindBuffer()
{
	glad_glBindBuffer(GL_ARRAY_BUFFER, VBO); //set my buffer as the bound buffer
}