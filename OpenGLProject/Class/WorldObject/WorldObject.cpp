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
	this->markedForDelete = false;
	boundingBox = NULL;
	boundingBoxAngle = glm::vec3(-1,-1,-1);
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

glm::vec3* WorldObject::getBoundingBox()
{
	if (angle!=boundingBoxAngle) { //recalculate the bounding box
		if (boundingBox != NULL) //delete any existing bounding box
		{
			delete[] boundingBox;
			boundingBox = NULL;
		}
		//construct the rotation matrix
		glm::mat4 rotationMatrix = glm::mat4(1.0); //create identity matrix
		rotationMatrix = glm::scale(rotationMatrix, this->scale);
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle.x), glm::vec3{ 1,0,0 });
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle.y), glm::vec3{ 0,1,0 });
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle.z), glm::vec3{ 0,0,1 });

		//rotate the verts
		auto firstVert = glm::vec3(rotationMatrix * model->verts.at(0));
		boundingBox = new glm::vec3[8]{
			glm::vec3(firstVert.x,firstVert.y,firstVert.z),
			glm::vec3(firstVert.x,firstVert.y,firstVert.z),

			glm::vec3(firstVert.x,firstVert.y,firstVert.z),
			glm::vec3(firstVert.x,firstVert.y,firstVert.z),

			glm::vec3(firstVert.x,firstVert.y,firstVert.z),
			glm::vec3(firstVert.x,firstVert.y,firstVert.z),

			glm::vec3(firstVert.x,firstVert.y,firstVert.z),
			glm::vec3(firstVert.x,firstVert.y,firstVert.z)
		};
		//calc bounding box. get the min and maximum x, y, & z coordinates of all vertices in the all meshes
		for (size_t i = 1;i < model->verts.size();i++)
		{
			//rotate & scale the vert (we'll translate/reposition the final bounding box afterward)
			auto vert = glm::vec3(rotationMatrix * model->verts.at(i));
			boundingBox[0].x = std::min(boundingBox[0].x, vert.x);
			boundingBox[1].x = std::max(boundingBox[1].x, vert.x);
			boundingBox[2].x = std::min(boundingBox[2].x, vert.x);
			boundingBox[3].x = std::max(boundingBox[3].x, vert.x);
			boundingBox[4].x = std::min(boundingBox[4].x, vert.x);
			boundingBox[5].x = std::max(boundingBox[5].x, vert.x);
			boundingBox[6].x = std::min(boundingBox[6].x, vert.x);
			boundingBox[7].x = std::max(boundingBox[7].x, vert.x);

			boundingBox[0].y = std::max(boundingBox[0].y, vert.y);
			boundingBox[1].y = std::max(boundingBox[1].y, vert.y);
			boundingBox[2].y = std::max(boundingBox[2].y, vert.y);
			boundingBox[3].y = std::max(boundingBox[3].y, vert.y);
			boundingBox[4].y = std::min(boundingBox[4].y, vert.y);
			boundingBox[5].y = std::min(boundingBox[5].y, vert.y);
			boundingBox[6].y = std::min(boundingBox[6].y, vert.y);
			boundingBox[7].y = std::min(boundingBox[7].y, vert.y);

			boundingBox[0].z = std::min(boundingBox[0].z, vert.z);
			boundingBox[1].z = std::min(boundingBox[1].z, vert.z);
			boundingBox[2].z = std::max(boundingBox[2].z, vert.z);
			boundingBox[3].z = std::max(boundingBox[3].z, vert.z);
			boundingBox[4].z = std::min(boundingBox[4].z, vert.z);
			boundingBox[5].z = std::min(boundingBox[5].z, vert.z);
			boundingBox[6].z = std::max(boundingBox[6].z, vert.z);
			boundingBox[7].z = std::max(boundingBox[7].z, vert.z);
		}
		//translate the bounding box
		for (int i = 0;i < 8;i++)
		{
			boundingBox[i].x -= (position.x);
			boundingBox[i].y -= (position.y);
			boundingBox[i].z += (position.z);
		}
	}
	return boundingBox;
}