#pragma once
#include <glad/glad.h>

class Texture
{
	public:
		Texture(const char* path);
		
		const char* path;
		unsigned int handle;
};