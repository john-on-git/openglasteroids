#version 410 core

layout (location = 1) in vec2 texCoordsIn;
layout (location = 0) in vec4 worldCoords;

uniform mat4 projection;
uniform mat4 modelView;

out vec2 texCoords;

void main()
{
	gl_Position = projection * modelView * worldCoords;
	texCoords = texCoordsIn;
}