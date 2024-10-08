﻿#pragma once

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


WorldObject::WorldObject(Model* model, glm::vec3 position, glm::vec3 angle, glm::vec3 scale, GLuint modelViewLocation, unordered_set<tag> tags)
{
	this->model = model;
	this->position = position;
	this->angle = angle;
	this->scale = scale;
	this->modelViewLocation = modelViewLocation;
	this->tags = tags;
	this->markedForDelete = false;
	this->colorMask = glm::vec4(1);
	boundingBox = nullptr;
	boundingBoxAngle = glm::vec3(-1, -1, -1);
	UpdateModelMatrix();
}

WorldObject::~WorldObject()
{
	delete[] boundingBox;
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
	//wraparound
	this->angle.x = (angle.x>=0 ? 0 : 360) + fmod(angle.x, 360);
	this->angle.y = (angle.y>=0 ? 0 : 360) + fmod(angle.y, 360);
	this->angle.z = (angle.z>=0 ? 0 : 360) + fmod(angle.z, 360);
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

void WorldObject::Draw(glm::mat4 viewMatrix)
{
	/*
* 
		22.5.22
		rotation works correctly
		translation does nothing but doesn't cause drawing errors
		projection breaks everything

		24.5.22
		it's working!
	*/
	//pass matrices to shader
		glad_glUniformMatrix4fv(
			modelViewLocation,
			1,
			GL_FALSE,
			glm::value_ptr(viewMatrix * this->modelMatrix)
		);
	//draw
	model->Draw(this->colorMask);
}

/**
* @return glm::vec3[8] containing the eight vertices of the object aligned bounding box. this pointer's memory is managed by the WorldObject. It should not be freed manually and should not be stored, as it may be become invalid at any time.
*/
glm::vec3* WorldObject::getOrientedBoundingBox()
{
	if (angle!=boundingBoxAngle) { //recalculate the bounding box
		//construct the rotation matrix

		//recalculate the rotated bounding box
		delete[] boundingBox;
		boundingBox = new glm::vec3[8];

		boundingBox[0] = glm::vec3(this->modelMatrix * glm::vec4(model->boundingMin.x, model->boundingMax.y, model->boundingMin.z, 1.0f));
		boundingBox[1] = glm::vec3(this->modelMatrix * glm::vec4(model->boundingMax.x, model->boundingMax.y, model->boundingMin.z, 1.0f));
		boundingBox[2] = glm::vec3(this->modelMatrix * glm::vec4(model->boundingMin.x, model->boundingMax.y, model->boundingMax.z, 1.0f));
		boundingBox[3] = glm::vec3(this->modelMatrix * glm::vec4(model->boundingMax.x, model->boundingMax.y, model->boundingMax.z, 1.0f));
		boundingBox[4] = glm::vec3(this->modelMatrix * glm::vec4(model->boundingMin.x, model->boundingMin.y, model->boundingMin.z, 1.0f));
		boundingBox[5] = glm::vec3(this->modelMatrix * glm::vec4(model->boundingMax.x, model->boundingMin.y, model->boundingMin.z, 1.0f));
		boundingBox[6] = glm::vec3(this->modelMatrix * glm::vec4(model->boundingMin.x, model->boundingMin.y, model->boundingMax.z, 1.0f));
		boundingBox[7] = glm::vec3(this->modelMatrix * glm::vec4(model->boundingMax.x, model->boundingMin.y, model->boundingMax.z, 1.0f));
	}
	return boundingBox;
}

glm::vec4* WorldObject::calcFaces()
{
	glm::vec4* faces = new glm::vec4[this->model->nFaces];
	for (size_t i = 0;i < this->model->nFaces;i++)
	{
		//the shift in position is probably what's breaking this.
		glm::mat3x3 face = this->model->faces[i]; //get the face representation
		//convert to vec4, applying transform (this breaks it)
		auto a = this->modelMatrix * glm::vec4(face[0], 1.0f);
		auto b = this->modelMatrix * glm::vec4(face[1], 1.0f);
		auto c = this->modelMatrix * glm::vec4(face[2], 1.0f);

		auto normal = glm::normalize(glm::cross(glm::vec3(b - a), glm::vec3(c - a)));
		auto d = -glm::dot(normal, glm::vec3(a));

		faces[i] = glm::vec4(normal, d);
	}
	return faces;
}

glm::mat2x4* WorldObject::calcEdges()
{
	glm::mat2x4* edges = new glm::mat2x4[this->model->nEdges];
	for (size_t i = 0; i < this->model->nEdges; i++)
	{
		glm::mat2x3 edge = this->model->edges[i]; //get the edge representation
		//TODO WHY ISN'T THE MATRIX WORKING 😭 (it was because the w component was 0)
		edges[i] = glm::mat2x4(
			this->modelMatrix * glm::vec4(edge[0], 1.0f),
			this->modelMatrix * glm::vec4(edge[1], 1.0f)
		);
	}
	return edges;
}