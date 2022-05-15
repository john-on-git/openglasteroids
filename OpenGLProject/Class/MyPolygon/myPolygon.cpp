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
#include "myPolygon.h"

MyPolygon::MyPolygon()
{
	OutputDebugStringW(L"FATAL: called invalid constructor MyPolygon()\n");
	exit(1);
}
MyPolygon::MyPolygon(float verts[], unsigned int length, glm::vec2 position, float angle)
{
	this->model = MyModel(verts, length);
	this->position = position;
	this->angle = angle;
}
MyPolygon::MyPolygon(std::string modelPath, glm::vec2 position, float angle)
{
	this->model = MyModel(modelPath);
	this->position = position;
	this->angle = angle;
}

void MyPolygon::Draw(GLint projectionLocation, GLint viewLocation, GLint modelLocation)
{
	glm::mat4 //new identity matrices
		projectionMatrix	= glm::mat4(1.0f),
		viewMatrix			= glm::mat4(1.0f),
		modelMatrix			= glm::mat4(1.0f);
	
	//calculate model matrix
	modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0));	//translate to the polygon's position
	modelMatrix = glm::rotate( //generate the matrix
		modelMatrix,
		glm::radians(angle),
		glm::vec3(0.0f, 0.0f, 1.0f)
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
	model.BindBuffer();
	glad_glDrawArrays(GL_TRIANGLE_STRIP, 0, this->model.nVertices); 
	/*
	doesn't work, because it's not immediate rendering
	this queues up many drawArrays, which all pull from the buffer of the last polygon that we said to draw
	so it draws the last poly many times
	stackEx says I need vertexAttribPointers or something? 
	https://stackoverflow.com/questions/23603228/draw-multiple-objects-from-different-buffers-arrays-switching
	that's enough OpenGL for one day, I think
	*/
}