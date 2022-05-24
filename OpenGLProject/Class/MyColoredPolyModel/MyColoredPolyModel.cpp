#include <windows.h>

#include "MyColoredPolyModel.hpp"
#include <glm/gtc/type_ptr.hpp>

MyColoredPolyModel::MyColoredPolyModel()
{
	OutputDebugStringW(L"FATAL: called not-implemented constructor MyColoredPolyModel()\n");
	exit(1);
}

MyColoredPolyModel::MyColoredPolyModel(size_t nPolygons, float* verts, glm::vec3* polygonColors, unsigned char* polygonBoundaries, GLuint colorLocation, GLuint VAO) : MyModel(nPolygons, verts, polygonBoundaries, VAO)
{
	this->polygonColors = polygonColors;
	this->colorLocation = colorLocation;
}

MyColoredPolyModel::MyColoredPolyModel(std::string, GLuint VAO)
{
	OutputDebugStringW(L"FATAL: called not-implemented constructor MyModel(std::string path)\n");
	exit(1);
}

void MyColoredPolyModel::Draw()
{
	//bind the buffer
		glad_glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	//draw every polygon
		size_t start = 0;
		for (size_t i = 0;i < nPolygons;i++)
		{
			//set color uniform value
			glad_glUniform3fv(
				colorLocation,
				1,
				glm::value_ptr(polygonColors[i])
			);
			//draw
			glad_glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			glad_glEnableVertexAttribArray(0);
			glad_glDrawArrays(GL_TRIANGLE_STRIP, start, polygonBoundaries[i]);
			start += polygonBoundaries[i];
		}
}