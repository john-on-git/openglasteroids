#pragma once

#include "../Texture/Texture.hpp"
#include <glm/ext/vector_float2.hpp>

class Renderer2D {
	protected:
		Texture* texture;
	public:
		void Draw(glm::vec2 position, glm::vec2 size);
		Renderer2D(Texture* texture);
};