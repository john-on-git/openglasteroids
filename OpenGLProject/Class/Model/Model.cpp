#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>

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
		for (size_t j = 0;j < aiMeshes[i]->mNumVertices;j++)
		{
			//also update the bounding box
			auto vert = aiMeshes[i]->mVertices[j];
			boundingMin.x = std::min(boundingMin.x, vert.x);
			boundingMax.x = std::max(boundingMax.x, vert.x);

			boundingMin.y = std::max(boundingMin.y, vert.y);
			boundingMax.y = std::max(boundingMax.y, vert.y);

			boundingMin.z = std::min(boundingMin.z, vert.z);
			boundingMax.z = std::max(boundingMax.z, vert.z);
		}
	}
}

void Model::Draw()
{
	for (size_t i = 0;i < numMeshes;i++)
		meshes[i].Draw();
}