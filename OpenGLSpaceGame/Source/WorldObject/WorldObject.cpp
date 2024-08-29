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
#include <iostream>

WorldObject::WorldObject(Model* model, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, GLuint projectionLocation, GLuint viewLocation, GLuint modelLocation, vector<tag> tags)
{
	this->model = model;
	this->position = position;
	this->angle = angle;
	this->scale = scale;
	this->projectionLocation = projectionLocation;
	this->viewLocation = viewLocation;
	this->modelLocation = modelLocation;
	this->tags = tags;
	this->markedForDelete = false;
	boundingBox = nullptr;
	boundingBoxAngle = glm::vec3(-1, -1, -1);
	UpdateModelMatrix();
}

void WorldObject::UpdateModelMatrix()
{
	this->modelMatrix = glm::mat4(1.0); //create identity matrix
	this->modelMatrix = glm::translate(this->modelMatrix, position);
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(angle.x), glm::vec3(1, 0, 0));
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(angle.y), glm::vec3(0, 1, 0));
	this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(angle.z), glm::vec3(0, 0, 1));
	this->modelMatrix = glm::scale(this->modelMatrix, scale);
}

glm::vec3 WorldObject::getPosition()
{
	return this->position;
}
void WorldObject::setPosition(glm::vec3 position)
{
	this->position = position;
	UpdateModelMatrix();
}

glm::vec3 WorldObject::getAngle()
{
	return this->angle;
}
void WorldObject::setAngle(glm::vec3 angle)
{
	this->angle = angle;
	UpdateModelMatrix();
}

glm::vec3 WorldObject::getScale()
{
	return this->scale;
}
void WorldObject::setScale(glm::vec3 scale)
{
	this->scale = scale;
	UpdateModelMatrix();
}

void WorldObject::Tick() {}

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
		//THESE ARE IN REVERSE ORDER FOR A REASON. BECAUSE OPENGL USES COLUMN-MAJOR MATRICES.
		//projection
			//init with identity
			//glm::mat4 projectionMatrix = glm::ortho(
			//	1.0f, -1.0f,	//left-right
			//	1.0f, -1.0f,	//bottom-top
			//	1.0f, 100.0f	//near-far
			//);
			glm::mat4 projectionMatrix = glm::perspective(75.0f, 1.0f, 1.0f, 100.0f);
		//view
			//init with identity
			glm::mat4 viewMatrix(glm::lookAt(glm::vec3(0,0,0),glm::vec3(0,0,-1),glm::vec3(0,-1,0)));
		//model
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
			glm::value_ptr(this->modelMatrix)
		);
	//draw
	model->Draw();
}

glm::vec3* WorldObject::getObjectAlignedBoundingBox()
{
	if (angle!=boundingBoxAngle) { //recalculate the bounding box
		if (boundingBox != nullptr) //delete any existing bounding box
		{
			delete[] boundingBox;
			boundingBox = nullptr;
		}
		//construct the rotation matrix

		//calculate the rotated bounding box
		boundingBox = new glm::vec3[8];
		for (unsigned char i = 0;i < 8;i++)
		{
			glm::vec4 vert;
			switch (i)
			{
				case 0:
					vert = glm::vec4(model->boundingMin.x, model->boundingMax.y, model->boundingMin.z, 1.0f);
					break;
				case 1:
					vert = glm::vec4(model->boundingMax.x, model->boundingMax.y, model->boundingMin.z, 1.0f);
					break;
				case 2:
					vert = glm::vec4(model->boundingMin.x, model->boundingMax.y, model->boundingMax.z, 1.0f);
					break;
				case 3:
					vert = glm::vec4(model->boundingMax.x, model->boundingMax.y, model->boundingMax.z, 1.0f);
					break;
				case 4:
					vert = glm::vec4(model->boundingMin.x, model->boundingMin.y, model->boundingMin.z, 1.0f);
					break;
				case 5:
					vert = glm::vec4(model->boundingMax.x, model->boundingMin.y, model->boundingMin.z, 1.0f);
					break;
				case 6:
					vert = glm::vec4(model->boundingMin.x, model->boundingMin.y, model->boundingMax.z, 1.0f);
					break;
				case 7:
					vert = glm::vec4(model->boundingMax.x, model->boundingMin.y, model->boundingMax.z, 1.0f);
					break;
			}
			boundingBox[i] = glm::vec3(this->modelMatrix * vert);
		}
	}
	return boundingBox;
}

std::vector<glm::vec4>* WorldObject::calcFaces()
{
	auto faces = new std::vector<glm::vec4>();
	for (unsigned int i = 0;i < this->model->faces.size();i++)
	{
		//the shift in position is probably what's breaking this.
		glm::mat3x3 face = this->model->faces.at(i); //get the face representation
		//convert to vec4, applying transform (this breaks it)
		auto a = this->modelMatrix * glm::vec4(face[0], 1.0f);
		auto b = this->modelMatrix * glm::vec4(face[1], 1.0f);
		auto c = this->modelMatrix * glm::vec4(face[2], 1.0f);

		auto normal = glm::normalize(glm::cross(glm::vec3(b - a), glm::vec3(c - a)));
		auto d = -glm::dot(normal, glm::vec3(a));

		faces->push_back(glm::vec4(normal, d));
	}
	return faces;
}

std::vector<glm::mat2x4>* WorldObject::calcEdges()
{
	auto edges = new std::vector<glm::mat2x4>();
	for (unsigned int i = 0; i < this->model->edges.size(); i++)
	{
		glm::mat2x3 edge = this->model->edges.at(i); //get the edge representation
		//TODO WHY ISN'T THE MATRIX WORKING 😭 (it was because the w component was 0)
		auto a = this->modelMatrix * glm::vec4(edge[0], 1.0f);
		auto b = this->modelMatrix * glm::vec4(edge[1], 1.0f);
		edges->push_back(glm::mat2x4(a,b));
	}
	return edges;
}