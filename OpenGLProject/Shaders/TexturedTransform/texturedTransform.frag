#version 410 core

//the problem is that vertex attributes are not passed to the fragment shader

//in vec4 color; //this is being passed correctly
in vec2 texCoords; //this isn't... Because they need to have the same variable name!? WTF is this shit

uniform sampler2D tex;

out vec4 colorOut;

void main() {
	colorOut = vec4(texCoords.x, 0, texCoords.y, 1);
	colorOut = vec4(1f,0f,0f,1f);//texture(tex, texCoords);
}