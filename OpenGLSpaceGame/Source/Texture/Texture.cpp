#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <cstddef>
#include <string>

Texture::Texture(const char* path)
{
	GLsizei x, y;
	GLubyte* data = stbi_load(path, &x, &y, nullptr, STBI_rgb_alpha);

	GLuint handle;
	glad_glGenTextures(1, &handle);
	glad_glBindTexture(GL_TEXTURE_2D, handle);

	//not duplicates
	glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete data;

	this->handle = handle;
}

//https://freetype.org/freetype2/docs/glyphs/glyphs-7.html
Texture::Texture(FT_Bitmap bitmap)
{
	constexpr char STRIDE = 4;

	//convert to RGBA
	size_t dataLength = (static_cast<size_t>(bitmap.width) * bitmap.rows) * STRIDE;
	GLubyte* data = new GLubyte[dataLength]();
	size_t i;
	for (i = 0; (i*STRIDE+3) < dataLength; i++)
	{
		data[i * STRIDE + 0] = bitmap.buffer[i]; //R
		data[i * STRIDE + 1] = bitmap.buffer[i]; //G
		data[i * STRIDE + 2] = bitmap.buffer[i]; //B
		data[i * STRIDE + 3] = bitmap.buffer[i]; //A
	}

	stbi_write_bmp("Textures/TestOut.bmp", bitmap.width, bitmap.rows, 4, data);

	GLuint handle;
	glad_glGenTextures(1, &handle);
	glad_glBindTexture(GL_TEXTURE_2D, handle);

	//not duplicates
	glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmap.width, bitmap.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	this->handle = handle;
}