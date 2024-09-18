#pragma once

#include <glad/glad.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/vec4.hpp>
#include <freetype/freetype.h>
#include <string>

class TextBox {
	protected:
		GLuint VAO;
		GLuint buffers[2];

		GLuint textureHandle;
		GLuint textureLocation;
		GLuint translationLocation;
		GLuint colorMaskLocation;

		glm::mat4 translationMatrix;

		glm::mat2 bounds;

		size_t numChars;
	public:
		glm::vec4 colorMask;
		TextBox(std::string text, GLuint textureHandle, GLuint textureLocation, GLuint translationLocation, GLuint colorMaskLocation, glm::vec2 position, glm::vec2 size, glm::vec2 windowDimensions);
		void Draw();
		bool InBounds(glm::vec2 pos);
};