#pragma once

#include "WorldObject.hpp"
#include "windows.h"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../BufferedAiMesh/BufferedAiMesh.hpp"


WorldObject::WorldObject()
{
	OutputDebugStringW(L"FATAL: called invalid constructor MyWorldThing()\n");
	exit(1);
}
WorldObject::WorldObject(Model* model, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation, vector<tag>* tags)
{
	this->model = model;
	this->position = position;
	this->angle = angle;
	this->scale = scale;
	this->projectionLocation = projectionLocation;
	this->viewLocation = viewLocation;
	this->modelLocation = modelLocation;
	this->tags = tags;
}

WorldObject::~WorldObject()
{
	delete tags;
}

void WorldObject::Draw()
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
			//translate
				//22.5.22 this is broken for points on the origin
				modelMatrix = glm::translate(modelMatrix, position);
			//rotate
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
			//scale
				modelMatrix = glm::scale(
					modelMatrix,
					scale
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