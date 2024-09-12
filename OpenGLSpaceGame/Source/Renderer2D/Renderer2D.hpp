#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glad/glad.h>
#include <glm/ext/vector_float4.hpp>

class Renderer2D {
	protected:

		GLuint VAO;
		GLuint buffers[2];

		GLuint textureHandle;
		GLuint textureLocation;
	public:
		void Draw(glm::vec2 position, glm::vec2 size);
		Renderer2D(GLuint textureHandle, GLuint textTextureLocation);
};