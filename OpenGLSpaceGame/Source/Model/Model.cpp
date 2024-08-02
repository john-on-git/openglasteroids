#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>
#include <glm/mat4x4.hpp>

Model::Model(std::string path, GLuint textureLocation, GLuint colorLocation, std::vector<GLuint> textures, std::vector<glm::vec4> colorMasks, size_t numTextures)
{
	//load mesh
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	numMeshes = scene->mNumMeshes;
	//initialize the bounding box with the values of an arbitrary vert from the model
	auto firstVert = scene->mMeshes[0]->mVertices[0];
	boundingMin = glm::vec4(firstVert.x, firstVert.y, firstVert.z, 1.0f);
	boundingMax = glm::vec4(firstVert.x, firstVert.y, firstVert.z, 1.0f);
	//convert to buffered meshes
	aiMesh** aiMeshes = scene->mMeshes;
	meshes = new BufferedAiMesh[numMeshes];
	for (size_t i = 0;i < numMeshes;i++)
	{
		meshes[i] = BufferedAiMesh(aiMeshes[i], textures.at(i), colorMasks.at(i), textureLocation, colorLocation);
		//also store the verts here for collision stuff
		auto firstVert = aiMeshes[i]->mVertices[0];
		boundingMin.x = std::min(boundingMin.x, firstVert.x);
		boundingMax.x = std::max(boundingMax.x, firstVert.x);

		boundingMin.y = std::max(boundingMin.y, firstVert.y);
		boundingMax.y = std::max(boundingMax.y, firstVert.y);

		boundingMin.z = std::min(boundingMin.z, firstVert.z);
		boundingMax.z = std::max(boundingMax.z, firstVert.z);
		for (unsigned int j = 1;j < aiMeshes[i]->mNumVertices;j++)
		{
			//also update the bounding box
			auto secondVert = aiMeshes[i]->mVertices[j];
			boundingMin.x = std::min(boundingMin.x, secondVert.x);
			boundingMax.x = std::max(boundingMax.x, secondVert.x);

			boundingMin.y = std::max(boundingMin.y, secondVert.y);
			boundingMax.y = std::max(boundingMax.y, secondVert.y);

			boundingMin.z = std::min(boundingMin.z, secondVert.z);
			boundingMax.z = std::max(boundingMax.z, secondVert.z);

			//add the edge
			edges.push_back(glm::mat2x4(
				glm::vec4(firstVert.x,   firstVert.y,  firstVert.z, 0),
				glm::vec4(secondVert.x, secondVert.y, secondVert.z, 0)
			));
			firstVert = secondVert;
		}
		//add each face
		for (unsigned int j = 0;j < aiMeshes[i]->mNumFaces;j++)
		{
			aiFace face = aiMeshes[i]->mFaces[j];
			//get the normal vector for the face by taking the cross product of the first two edges
			auto a = aiMeshes[i]->mVertices[face.mIndices[0]];
			auto b = aiMeshes[i]->mVertices[face.mIndices[1]];
			auto c = aiMeshes[i]->mVertices[face.mIndices[2]];

			auto firstEdge = b - a;
			auto secondEdge = c - a;

			auto firstVec = glm::vec3(firstEdge.x, firstEdge.y, firstEdge.z);
			auto secondVec = glm::vec3(secondEdge.x, secondEdge.y, secondEdge.z);

			faces.push_back(glm::mat2x3(firstVec, secondVec));
		}
	}
}

void Model::Draw()
{
	for (size_t i = 0;i < numMeshes;i++)
		meshes[i].Draw();
}