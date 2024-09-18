#include "TextBox.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../Texture/Texture.hpp"


static constexpr char N_SUPPORTED_CHARS = '~' - ' ' + 1;
static constexpr char TEXTBOX_STRIDE = 4;

TextBox::TextBox(std::string text, GLuint textureHandle, GLuint textureLocation, GLuint translationLocation, GLuint colorMaskLocation, glm::vec2 position, glm::vec2 size, glm::vec2 windowDimensions)
{
	this->textureHandle = textureHandle;
	this->textureLocation = textureLocation;
	this->translationLocation = translationLocation;
	this->colorMaskLocation = colorMaskLocation;

	//build the translation matrix
	translationMatrix = glm::mat4(1);
	translationMatrix = glm::translate(translationMatrix, glm::vec3(position, 0));
	translationMatrix = glm::scale(translationMatrix, glm::vec3(size, 1));

	this->colorMask = glm::vec4(1, 1, 1, 1); //init color mask

	numChars = text.length();
	//build the verts, finding the place of each character on the texture atlas
	GLfloat* verts = new GLfloat[numChars * 16]; //16 = 4 (num unique verts in a two tri rectangle) * 4 (2 screen coords + 2 tex coords)
	GLushort* vertIndices = new GLushort[numChars * 6]; //6 = num verts in a two tri rectangle

	//TODO
	for (long long int i = 0; i < numChars; i++)
	{
		GLfloat j = (GLfloat)(text.at(i) - ' ');
		//+ 1/26

		//screen 1
		verts[i*16] = -1 + (2*i);
		verts[i*16+1] = -1;
		//texture 1
		verts[i*16+2] = (j / N_SUPPORTED_CHARS);
		verts[i*16+3] = 1.0f;


		//screen 2
		verts[i*16+4] = 1 + (2 * i);
		verts[i*16+5] = -1;

		//texture 2
		verts[i*16+6] = ((j + 1) / N_SUPPORTED_CHARS);
		verts[i*16+7] = 1.0f;


		//screen 3
		verts[i*16+8] = -1 + (2 * i);
		verts[i*16+9] = 1;

		//texture 3
		verts[i*16+10] = (j / N_SUPPORTED_CHARS);
		verts[i*16+11] = 0.0f;


		//screen 4
		verts[i*16+12] = 1 + (2 * i);
		verts[i*16+13] = 1;

		//texture 4
		verts[i*16+14] = ((j + 1) / N_SUPPORTED_CHARS);
		verts[i*16+15] = 0.0f;

		//first tri
		vertIndices[i*6+0] = i*4+0;
		vertIndices[i*6+1] = i*4 +1;
		vertIndices[i*6+2] = i*4 +2;

		///second tri
		vertIndices[i*6+3] = i*4+1;
		vertIndices[i*6+4] = i*4+2;
		vertIndices[i*6+5] = i*4+3;
	}
	//min coords
	bounds[0] = glm::vec2((((verts[0] * size.x) + position.x) + 1.0f) / 2 * windowDimensions.x, (((verts[1] * size.y) - position.y) + 1.0f) / 2 * windowDimensions.y);
	//max coords
	bounds[1] = glm::vec2((((verts[(numChars - 1) * 16 + 12] * size.x) + position.x) + 1.0f) / 2 * windowDimensions.x, (((verts[(numChars - 1) * 16 + 13] * size.y) - position.y) + 1.0f) / 2 * windowDimensions.y);

	//render each glyph, copying it over to the texture atlas

	//generate vertex array object
	glad_glGenVertexArrays(1, &VAO);
	glad_glBindVertexArray(VAO);

	//generate buffers and copy over data
	glad_glGenBuffers(2, buffers);
	//vertex buffer
	glad_glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glad_glBufferData( //add verts to buffer
		GL_ARRAY_BUFFER,
		text.length() * 16 * sizeof(GLfloat),
		verts,
		GL_STATIC_DRAW
	);
	//vertex index buffer
	glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glad_glBufferData( //add verts to buffer
		GL_ELEMENT_ARRAY_BUFFER,
		text.length() * 6 * sizeof(GLushort),
		vertIndices,
		GL_STATIC_DRAW
	);
		
	//set vertex attrib pointers
	glad_glVertexAttribPointer( //position
		0,
		2,
		GL_FLOAT,
		GL_FALSE,
		TEXTBOX_STRIDE * sizeof(GLfloat),
		nullptr
	);
	glad_glEnableVertexAttribArray(0);

	glad_glVertexAttribPointer( //texCoord
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		TEXTBOX_STRIDE * sizeof(GLfloat),
		(GLvoid*)(2 * sizeof(GLfloat)) //offset
	);
	glad_glEnableVertexAttribArray(1);
}

void TextBox::Draw()
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
	glad_glUniform4fv(colorMaskLocation, 1, glm::value_ptr(colorMask));

	glad_glDrawElements(
		GL_TRIANGLES,
		numChars * 16,
		GL_UNSIGNED_SHORT,
		nullptr
	);
}

bool TextBox::InBounds(glm::vec2 pos)
{
	return
		pos.x > bounds[0].x &&
		pos.y > bounds[0].y &&
		pos.x < bounds[1].x &&
		pos.y < bounds[1].y;
}