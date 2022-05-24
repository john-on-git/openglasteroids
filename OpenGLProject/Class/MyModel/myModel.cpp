#include <windows.h>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <iostream>

#include "myModel.hpp"
#include "../../main.hpp"

MyModel::MyModel()
{
	nPolygons = 0;
	VBOs = NULL;
	polygonBoundaries = NULL;
}
/// <summary>
/// manually create a model in code
/// </summary>
/// <param name="nPolygons">number of polygons in the model</param>
/// <param name="verts">the coordinates of the model's vertices</param>
/// <param name="polygonBoundaries">the number of verts in each polygon, in order, with a leading zero</param>
/// <param name="VAO">the VAO that include this model's VBO will be entered into</param>
MyModel::MyModel(size_t nPolygons, float *verts, unsigned char *polygonBoundaries, GLuint VAO)
{
	//init properties
		this->nPolygons = nPolygons;
		VBOs = new GLuint[1];
		this->polygonBoundaries = polygonBoundaries;
	//get total number of verts
		size_t nVerts = 0;
		for (size_t i = 0;i < nPolygons;i++)
			nVerts += polygonBoundaries[i];

		//std::cout << "verts:\n";
		//for (size_t i = 0;i < nVerts * 4;i+=4)
		//{
		//	std::cout << "\t["
		//				<< verts[i]
		//		<< ", " << verts[i+1]
		//		<< ", " << verts[i+2]
		//		<< ", " << verts[i+3]
		//		<< "]\n";
		//}
	//generate, bind, copy verts to my buffer
		glad_glBindVertexArray(VAO);
		glad_glGenBuffers(1, VBOs); //19.5.21, first argument is the number of buffers, not the size of the buffer. corrupted the heap?
		glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glad_glBufferData( //add verts to buffer
			GL_ARRAY_BUFFER,
			nVerts * sizeof(float) * 4,
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
	return;
	//bind the buffer
		glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	//draw every polygon
		size_t start = 0;
		for (size_t i = 0;i < nPolygons;i++)
		{
			glad_glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glad_glEnableVertexAttribArray(0);
			glad_glDrawArrays(GL_TRIANGLE_STRIP, start, polygonBoundaries[i]);
			start += polygonBoundaries[i];
		}
}