#pragma once

#include "../Texture/Texture.hpp"
#include <glm/ext/vector_float2.hpp>
#include <glad/glad.h>
#include <glm/ext/vector_float4.hpp>

class Renderer2D {
	protected:
		Texture* texture;

		GLuint VAO;
		GLuint buffers[2];

		GLuint textTextureLocation;

		static constexpr char NUM_VERTS = 6;
		static constexpr char STRIDE = 4; //vec2 vert coords + vec2 texture coords
		static constexpr GLfloat VERTS[NUM_VERTS * STRIDE] = {
			0,0, 0,0,	1,0, 1,0,
			0,1, 0,1,	1,1, 1,1,				 
		};
		static constexpr GLushort VERT_INDICES[NUM_VERTS] = { 0,1,2,	1,2,3 };
	public:
		void Draw(glm::vec2 position, glm::vec2 size);
		Renderer2D(Texture* texture, GLuint textTextureLocation);
};