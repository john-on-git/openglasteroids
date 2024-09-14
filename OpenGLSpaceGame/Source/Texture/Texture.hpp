#pragma once
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>

class Texture
{
	public:
		Texture(const char* path);
		Texture(FT_Bitmap bitmap);
		Texture(GLubyte* data, size_t width, size_t height);
		
		GLuint handle;
};