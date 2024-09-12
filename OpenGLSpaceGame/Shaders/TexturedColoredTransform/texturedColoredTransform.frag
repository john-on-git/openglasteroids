#version 410 core

//the problem is that vertex attributes are not passed to the fragment shader

in vec2 texCoords; //this isn't... Because they need to have the same variable name!?

uniform sampler2D tex;
uniform vec4 colorMask;

out vec4 colorOut;

void main() {
	colorOut = texture(tex, texCoords) * colorMask;
}