#include "Renderer2D.hpp"

void Renderer2D::Draw(glm::vec2 position, glm::vec2 size)
{
	glRasterPos2d(position.x, position.y);
	//glDrawPixels(size.x,size.y, GL_RGB, GL_UNSIGNED_BYTE, texture->handle);
	glad_glBindTexture(GL_TEXTURE_2D, texture->handle);
	//TODO make a square, use the texture, draw it in 2D
}

Renderer2D::Renderer2D(Texture* texture)
{
	this->texture = texture;
}
