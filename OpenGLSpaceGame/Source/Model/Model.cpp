#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>
#include <glm/mat4x4.hpp>

Model::Model(std::string path, GLuint textureLocation, GLuint colorLocation, std::vector<GLuint> textures, size_t numTextures)
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
	meshes = new BufferedAiMesh*[numMeshes];
	auto edgeVector = std::vector<glm::mat2x3>();
	auto faceVector = std::vector<glm::mat3x3>();
	for (size_t i = 0;i < numMeshes;i++)
	{
		meshes[i] = new BufferedAiMesh(aiMeshes[i], textures.at(i), textureLocation, colorLocation);
		//also store the verts here for collision stuff
		auto firstVert = aiMeshes[i]->mVertices[0];
		boundingMin.x = std::min(boundingMin.x, firstVert.x);
		boundingMax.x = std::max(boundingMax.x, firstVert.x);

		boundingMin.y = std::max(boundingMin.y, firstVert.y);
		boundingMax.y = std::max(boundingMax.y, firstVert.y);

		boundingMin.z = std::min(boundingMin.z, firstVert.z);
		boundingMax.z = std::max(boundingMax.z, firstVert.z);
		for (size_t j = 1;j < aiMeshes[i]->mNumVertices;j++)
		{
			//also update the bounding box
			auto secondVert = aiMeshes[i]->mVertices[j];
			boundingMin.x = std::min(boundingMin.x, secondVert.x);
			boundingMax.x = std::max(boundingMax.x, secondVert.x);

			boundingMin.y = std::max(boundingMin.y, secondVert.y);
			boundingMax.y = std::max(boundingMax.y, secondVert.y);

			boundingMin.z = std::min(boundingMin.z, secondVert.z);
			boundingMax.z = std::max(boundingMax.z, secondVert.z);

			//add the edge (discarding any zero-length edges)
			auto edge = glm::mat2x3(
				glm::vec3(firstVert.x, firstVert.y, firstVert.z),
				glm::vec3(secondVert.x, secondVert.y, secondVert.z)
			);
			if (glm::length(edge[1] - edge[0]) > 0)
			{
				edgeVector.push_back(edge);
				nEdges++;
			}
			firstVert = secondVert;
		}
		//add each face
		for (size_t j = 0;j < aiMeshes[i]->mNumFaces;j++)
		{
			aiFace face = aiMeshes[i]->mFaces[j];
			auto a = aiMeshes[i]->mVertices[face.mIndices[0]];
			auto b = aiMeshes[i]->mVertices[face.mIndices[1]];
			auto c = aiMeshes[i]->mVertices[face.mIndices[2]];

			faceVector.push_back(glm::mat3x3(glm::vec3(a.x,a.y,a.z), glm::vec3(b.x,b.y,b.z), glm::vec3(c.x,c.y,c.z)));
			nFaces++;
		}
	}
	edges = new glm::mat2x3[nEdges];
	for (size_t i = 0; i < edgeVector.size(); i++)
	{
		edges[i] = edgeVector.at(i);
	}
	faces = new glm::mat3x3[nFaces];
	for (size_t i = 0; i < faceVector.size(); i++)
	{
		faces[i] = faceVector.at(i);
	}
}

Model::~Model()
{
	delete[] meshes;
	delete[] edges;
	delete[] faces;
}

void Model::Draw(glm::vec4 colorMask)
{
	for (size_t i = 0;i < numMeshes;i++)
	{
		meshes[i]->Draw(colorMask);
	}
}