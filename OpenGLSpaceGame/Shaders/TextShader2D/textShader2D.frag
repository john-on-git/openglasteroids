#version 410 core

in vec4 colorMask; //this is being passed correctly
in vec2 texCoords; //this isn't... Because they need to have the same variable name!?

uniform sampler2D tex;

out vec4 colorOut;

void main() {
	colorOut = texture(tex, texCoords) * colorMask;
}