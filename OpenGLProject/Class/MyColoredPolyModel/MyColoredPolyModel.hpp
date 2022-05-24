#pragma once

#include <glm/vec3.hpp>
#include "../MyModel/MyModel.hpp"

class MyColoredPolyModel : public MyModel
{
	public:
		MyColoredPolyModel();
		MyColoredPolyModel(size_t nPolygons, float *verts, glm::vec3* polygonColors, unsigned char *polygonBoundaries, GLuint colorLocation, GLuint VAO);
		MyColoredPolyModel(std::string, GLuint VAO);

		void Draw();
	protected:
		GLuint colorLocation;
		glm::vec3 *polygonColors; //number of verts in each polygon
};
