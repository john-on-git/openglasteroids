#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <freetype/freetype.h>
#include <string>

class TextBox {
	protected:

		GLuint VAO;
		GLuint buffers[2];

		GLuint textureHandle;
		GLuint textureLocation;
		GLuint translationLocation;

		glm::mat4 translationMatrix;
	public:
		void Draw();
		TextBox(std::string text, GLuint textureHandle, GLuint textureLocation, GLuint translationLocation, glm::vec2 position, glm::vec2 size);
};