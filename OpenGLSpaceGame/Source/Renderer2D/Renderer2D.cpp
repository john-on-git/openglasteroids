#include "Renderer2D.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Renderer2D::Draw(glm::vec2 position, glm::vec2 size)
{
	glad_glBindVertexArray(VAO);

	glad_glActiveTexture(GL_TEXTURE0);
	glad_glBindTexture(GL_TEXTURE_2D, texture->handle);

	glad_glUniform1i(textTextureLocation, 0);
	//draw (crashes)
	glad_glDrawElements(
		GL_TRIANGLES,
		NUM_VERTS,
		GL_UNSIGNED_SHORT,
		nullptr
	);
}

Renderer2D::Renderer2D(Texture* texture, GLuint textTextureLocation)
{
	this->texture = texture;
	this->textTextureLocation = textTextureLocation;

	//generate vertex array object
	glad_glGenVertexArrays(1, &VAO);
	glad_glBindVertexArray(VAO);

	//generate buffers and copy over data
	glad_glGenBuffers(2, buffers);
	//vertex buffer
	glad_glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glad_glBufferData( //add verts to buffer
		GL_ARRAY_BUFFER,
		static_cast<size_t>(NUM_VERTS) * STRIDE * sizeof(GLfloat),
		VERTS,
		GL_STATIC_DRAW
	);
	//vertex index buffer
	glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glad_glBufferData( //add verts to buffer
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<size_t>(NUM_VERTS) * sizeof(GLshort),
		VERTS,
		GL_STATIC_DRAW
	);
		
	//set vertex attrib pointers
	glad_glEnableVertexAttribArray(0);
	glad_glVertexAttribPointer( //position
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		STRIDE * sizeof(GLfloat),
		nullptr
	);

	glad_glEnableVertexAttribArray(1);
	glad_glVertexAttribPointer( //texCoord
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		STRIDE * sizeof(GLfloat),
		(GLvoid*)(2 * sizeof(GLfloat)) //offset
	);

}
