#pragma once
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class Texture
{
	public:
		Texture(const char* path);
		Texture(FT_Bitmap bitmap);
		
		GLuint handle;
};