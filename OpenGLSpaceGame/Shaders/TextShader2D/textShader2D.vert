#version 410 core

layout (location = 0) in vec2 screenCoords;
layout (location = 1) in vec2 texCoordsIn;

uniform mat4 translation;

out vec2 texCoords;

void main()
{
	gl_Position = translation * vec4(screenCoords, 0, 1);
	texCoords = texCoordsIn;
}