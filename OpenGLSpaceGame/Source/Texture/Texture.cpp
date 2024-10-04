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

Texture::Texture(GLubyte* data, size_t width, size_t height)
{
	GLuint handle;
	glad_glGenTextures(1, &handle);
	glad_glBindTexture(GL_TEXTURE_2D, handle);

	//not duplicates
	glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	this->handle = handle;
}