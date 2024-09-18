#version 410 core

in vec2 texCoords;

uniform sampler2D tex;
uniform vec4 colorMask;

out vec4 colorOut;

void main() {
	colorOut = colorMask * texture(tex, texCoords);
}