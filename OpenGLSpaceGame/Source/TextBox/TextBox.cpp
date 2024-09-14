#include "TextBox.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "../Texture/Texture.hpp"


static constexpr char NUM_VERTS = 6;
static constexpr char TEXTBOX_STRIDE = 4; //vec2 vert coords + vec2 texture coords
static constexpr GLfloat VERTS[NUM_VERTS * TEXTBOX_STRIDE] = {
	-1,-1, 0,1,		1,-1, 1,1,
	-1, 1, 0,0,		1, 1, 1,0,
};
static constexpr GLushort VERT_INDICES[NUM_VERTS] = { 0,1,2,	1,2,3 };

TextBox::TextBox(std::string text, GLuint textureHandle, GLuint textureLocation, GLuint translationLocation, glm::vec2 position, glm::vec2 size)
{
	this->textureHandle = textureHandle;
	this->textureLocation = textureLocation;
	this->translationLocation = translationLocation;

	//build the translation matrix
	translationMatrix = glm::mat4(1);
	translationMatrix = glm::translate(translationMatrix, glm::vec3(position, 0));
	translationMatrix = glm::scale(translationMatrix, glm::vec3(size, 1));

	//build the verts, finding the place of each character on the texture atlas
	GLfloat* verts = new GLfloat[text.length() * 16]; //16 = 4 (num unique verts in a two tri rectangle) * 4 (2 screen coords + 2 tex coords)
	GLubyte* vertIndices = new GLubyte[text.length() * 6]; //6 = num verts in a two tri rectangle

	//TODO
	for (long long int i = 0; i < text.length(); i++)
	{
		GLfloat j = (GLfloat)(text.at(i) - 'A');
		//+ 1/26

		//screen 1
		verts[i*16] = -1 + (2*i);
		verts[i*16+1] = -1 + (2*i);
		//texture 1
		verts[i*16+2] = j*(1.0f/26.0f);
		verts[i*16+3] = 1.0f;


		//screen 2
		verts[i*16+4] = 1 + (2 * i);
		verts[i*16+5] = -1 + (2 * i);

		//texture 2
		verts[i*16+6] = (j+1) * (1.0f / 26.0f);
		verts[i*16+7] = 1.0f;


		//screen 3
		verts[i*16+8] = -1 + (2 * i);
		verts[i*16+9] = 1 + (2 * i);

		//texture 3
		verts[i*16+10] = j * (1.0f / 26.0f);
		verts[i*16+11] = 0.0f;


		//screen 4
		verts[i*16+12] = 1 + (2 * i);
		verts[i*16+13] = 1 + (2 * i);

		//texture 4
		verts[i*16+14] = (j+1) * (1.0f / 26.0f);
		verts[i*16+15] = 0.0f;

		//first tri
		vertIndices[i*6+0] = i;
		vertIndices[i*6+1] = i+1;
		vertIndices[i*6+2] = i+2;

		///second tri
		vertIndices[i*6+3] = i+3;
		vertIndices[i*6+4] = i+4;
		vertIndices[i*6+5] = i+5;
	}

	for (size_t i = 0; i < text.length() * 16; i+=4)
	{
		std::cerr << verts[i] << ' ' << verts[i+1] << ' ' << verts[i+2] << ' ' << verts[i+3] << ' ' << std::endl;
	}

	//(!ftMainFont->size)
	//FT_Set_Char_Size just calculates some values, then hands control to another function FT_Request_Size, which immediately errors if the above condition is false
	//no idea why, the purpose of the function is to init Face->size, why would you first check if it's already init?
	//docs: "When the FT_New_Face function is called (or one of its siblings), it automatically creates a new size object for the returned face. This size object is directly accessible as face->size."
	//So I guess it's a bug?
	//it was bc the index in FT_New_Face was set to -1, docs say 0. -1 is for checking the number of faces
	
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
		text.length() * 6 * sizeof(GLubyte),
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

	glad_glDrawElements(
		GL_TRIANGLE_STRIP,
		NUM_VERTS,
		GL_UNSIGNED_SHORT,
		nullptr
	);
}
