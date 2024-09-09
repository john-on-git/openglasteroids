#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <cstddef>
#include <string>

Texture::Texture(const char* path)
{
	int x, y;
	unsigned char* data = stbi_load(path, &x, &y, nullptr, STBI_rgb_alpha);

	unsigned int handle;
	glad_glGenTextures(1, &handle);
	glad_glBindTexture(GL_TEXTURE_2D, handle);
	glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete data;

	this->path = path;
	this->handle = handle;
}

Texture::Texture(FT_Bitmap bitmap)
{
	int x = bitmap.width;
	int y = bitmap.rows;
	unsigned char* data = bitmap.buffer;

	/*for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			std::cerr << std::to_string((int)data[i + (i * y) + j]) << " ";
		}
		std::cerr << std::endl;
	}*/

	unsigned int handle;
	glad_glGenTextures(1, &handle);
	glad_glBindTexture(GL_TEXTURE_2D, handle);
	glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glad_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	this->path = path;
	this->handle = handle;
}