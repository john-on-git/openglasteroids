#version 330 core

uniform vec3 color;

out vec4 colorOut;

void main() {
	colorOut = vec4(color.x, color.y, color.z, 1f);
}