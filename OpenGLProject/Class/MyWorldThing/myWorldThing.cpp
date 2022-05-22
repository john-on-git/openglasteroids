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
MyWorldThing::MyWorldThing(float verts[], unsigned int length, glm::vec3 position, glm::vec3 angle, GLint VAO)
{
	this->model = MyModel(verts, length, VAO);
	this->position = position;
	this->angle = angle;
}
MyWorldThing::MyWorldThing(std::string modelPath, glm::vec3 position, glm::vec3 angle, GLint VAO)
{
	this->model = MyModel(modelPath, VAO);
	this->position = position;
	this->angle = angle;
}

void MyWorldThing::Draw(GLint projectionLocation, GLint viewLocation, GLint modelLocation)
{
	//calculate matrices
		glm::mat4 projectionMatrix(1.0f);
		glm::mat4 viewMatrix(1.0f);
		glm::mat4 modelMatrix(1.0f);
		//this is the broken part!
		//projection
		//	glm::mat4 projectionMatrix = glm::perspective(
		//		090.0f,
		//		001.0f,
		//		100.0f,
		//		000.01f
		//	);
		////view
		//	//init with identity
		//		glm::mat4 viewMatrix(1.0f);
		////model
		//	//init with identity
		//		glm::mat4 modelMatrix(1.0f);
		//	//rotate around each axis
		//		modelMatrix = glm::rotate(
		//			modelMatrix,
		//			glm::radians(angle.x),
		//			glm::vec3(1,0,0)
		//		);
		//		modelMatrix = glm::rotate(
		//			modelMatrix,
		//			glm::radians(angle.y),
		//			glm::vec3(0,1,0)
		//		);
		//		modelMatrix = glm::rotate(
		//			modelMatrix,
		//			glm::radians(angle.z),
		//			glm::vec3(0,0,1)
		//		);
		//	//translate to the polygon's position
		//	modelMatrix = glm::translate(modelMatrix, position);
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
	//draw
	model.Draw();
}