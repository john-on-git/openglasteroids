#pragma once

#include <glad/glad.h>
//glm
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <windows.h>

#include "../MyModel/myModel.h"
#include "myWorldThing.h"

MyWorldThing::MyWorldThing()
{
	OutputDebugStringW(L"FATAL: called invalid constructor MyWorldThing()\n");
	exit(1);
}
MyWorldThing::MyWorldThing(float verts[], unsigned int length, glm::vec3 position, glm::vec4 angle)
{
	this->model = MyModel(verts, length);
	this->position = position;
	this->angle = angle;
}
MyWorldThing::MyWorldThing(std::string modelPath, glm::vec3 position, glm::vec4 angle)
{
	this->model = MyModel(modelPath);
	this->position = position;
	this->angle = angle;
}

void MyWorldThing::Draw(GLint projectionLocation, GLint viewLocation, GLint modelLocation)
{
	glm::mat4 //new identity matrices
		projectionMatrix	= glm::mat4(1.0f),
		viewMatrix			= glm::mat4(1.0f),
		modelMatrix			= glm::mat4(1.0f);
	
	//calculate model matrix
	modelMatrix = glm::translate(modelMatrix, position);	//translate to the polygon's position
	modelMatrix = glm::rotate( //generate the matrix
		modelMatrix,
		glm::radians(angle[0]),
		glm::vec3(
			angle[1],
			angle[2],
			angle[3]
		)
	);

	//pass matrices to shader
	glad_glUniformMatrix4fv(
		projectionLocation,
		1,
		GL_FALSE,
		glm::value_ptr(projectionMatrix)
	);
	glad_glUniformMatrix4fv(
		viewLocation,
		1,
		GL_FALSE,
		glm::value_ptr(viewMatrix)
	);
	glad_glUniformMatrix4fv(
		modelLocation,
		1,
		GL_FALSE,
		glm::value_ptr(modelMatrix)
	);

	glad_glDrawArrays(GL_TRIANGLE_STRIP, 0, this->model.nVertices); 
	/*
	doesn't work, because it's not immediate rendering
	this queues up many drawArrays, which all pull from the buffer of the last polygon that we said to draw
	so it draws the last poly many times
	stackEx says I need vertexAttribPointers or something? 
	https://stackoverflow.com/questions/23603228/draw-multiple-objects-from-different-buffers-arrays-switching
	that's enough OpenGL for one day, I think
	*/

	//why is it still not working ;o;
}