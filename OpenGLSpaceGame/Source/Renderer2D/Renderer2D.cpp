#include "Renderer2D.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


static constexpr char NUM_VERTS = 6;
static constexpr char STRIDE = 4; //vec2 vert coords + vec2 texture coords
static constexpr GLfloat VERTS[NUM_VERTS * STRIDE] = {
	-1,-1, 0,1,		1,-1, 1,1,
	-1, 1, 0,0,		1, 1, 1,0,
};
static constexpr GLushort VERT_INDICES[NUM_VERTS] = { 0,1,2,	1,2,3 };

Renderer2D::Renderer2D(GLuint textureHandle, GLuint textureLocation, GLuint translationLocation, glm::vec2 position, glm::vec2 size)
{
	this->textureHandle = textureHandle;
	this->textureLocation = textureLocation;
	this->translationLocation = translationLocation;

	//build the translation matrix
	translationMatrix = glm::mat4(1);
	translationMatrix = glm::translate(translationMatrix, glm::vec3(position, 0));
	translationMatrix = glm::scale(translationMatrix, glm::vec3(size, 1));

	//generate vertex array object
	glad_glGenVertexArrays(1, &VAO);
	glad_glBindVertexArray(VAO);

	//generate buffers and copy over data
	glad_glGenBuffers(2, buffers);
	//vertex buffer
	glad_glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glad_glBufferData( //add verts to buffer
		GL_ARRAY_BUFFER,
		sizeof(VERTS),
		VERTS,
		GL_STATIC_DRAW
	);
	//vertex index buffer
	glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glad_glBufferData( //add verts to buffer
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(VERT_INDICES),
		VERT_INDICES,
		GL_STATIC_DRAW
	);
		
	//set vertex attrib pointers
	glad_glVertexAttribPointer( //position
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		STRIDE * sizeof(GLfloat),
		nullptr
	);
	glad_glEnableVertexAttribArray(0);

	glad_glVertexAttribPointer( //texCoord
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		STRIDE * sizeof(GLfloat),
		(GLvoid*)(2 * sizeof(GLfloat)) //offset
	);
	glad_glEnableVertexAttribArray(1);
}

void Renderer2D::Draw()
{
	glad_glBindVertexArray(VAO);

	glad_glActiveTexture(GL_TEXTURE0);
	glad_glBindTexture(GL_TEXTURE_2D, textureHandle);

	glad_glUniform1i(textureLocation, 0);
	glad_glUniformMatrix4fv(
		translationLocation,
		1,
		GL_FALSE,
		glm::value_ptr(translationMatrix)
	);

	glad_glDrawElements(
		GL_TRIANGLES,
		NUM_VERTS,
		GL_UNSIGNED_SHORT,
		nullptr
	);
}
