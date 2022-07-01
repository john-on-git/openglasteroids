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

#include "../BufferedAiMesh/BufferedAiMesh.hpp"
#include "myWorldThing.hpp"

MyWorldThing::MyWorldThing()
{
	OutputDebugStringW(L"FATAL: called invalid constructor MyWorldThing()\n");
	exit(1);
}
MyWorldThing::MyWorldThing(BufferedAiMesh *model, glm::vec3 position, glm::vec3 angle, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation)
{
	this->model = model;
	this->position = position;
	this->angle = angle;
	this->projectionLocation = projectionLocation;
	this->viewLocation = viewLocation;
	this->modelLocation = modelLocation;
}

void MyWorldThing::Draw()
{
	/*
		22.5.22
		rotation works correctly
		translation does nothing but doesn't cause drawing errors
		projection breaks everything

		24.5.22
		it's working!
	*/
	//calculate matrices
		//projection
			//this is the broken part!
			glm::mat4 projectionMatrix = glm::perspective(
				75.0f,
				1.0f,
				0.1f,
				100.0f
			);
		//view
			//init with identity
				glm::mat4 viewMatrix(1.0f);
		//model
			//init with identity
				glm::mat4 modelMatrix(1.0f);
			//translate to the polygon's position
				//22.5.22 this is broken for points on the origin
				modelMatrix = glm::translate(modelMatrix, position);
			//rotate around each axis
				modelMatrix = glm::rotate(
					modelMatrix,
					glm::radians(angle.x),
					glm::vec3(1,0,0)
				);
				modelMatrix = glm::rotate(
					modelMatrix,
					glm::radians(angle.y),
					glm::vec3(0,1,0)
				);
				modelMatrix = glm::rotate(
					modelMatrix,
					glm::radians(angle.z),
					glm::vec3(0,0,1)
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
	//draw
	model->Draw();
}