#pragma once

#include <glm/vec3.hpp>
#include "../MyModel/MyModel.hpp"

class MyTexturedModel : public MyModel
{
	public:
		MyTexturedModel();
		MyTexturedModel(size_t nPolygons, float *verts, GLuint *textures, unsigned char *polygonBoundaries, GLuint colorLocation, GLuint VAO);
		MyTexturedModel(std::string, GLuint VAO);

		void Draw();
	protected:
		GLuint textureLocation;
		GLuint *textures;
};
