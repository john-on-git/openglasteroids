#version 410 core

in vec2 texCoords;

uniform sampler2D tex;

out vec4 colorOut;

void main() {
	colorOut = texture(tex, texCoords);
}