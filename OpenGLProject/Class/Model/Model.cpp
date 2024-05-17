#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>

Model::Model(std::string path, GLuint textureLocation, GLuint colorLocation, GLuint* textures, glm::vec4* colorMasks, size_t numTextures)
{
	//load mesh
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	numMeshes = scene->mNumMeshes;
	//initialize the bounding box with the values of an arbitrary vert from the model
	auto firstVert = scene->mMeshes[0]->mVertices[0];
	//convert to buffered meshes
	aiMesh** aiMeshes = scene->mMeshes;
	meshes = new BufferedAiMesh[numMeshes];
	for (size_t i = 0;i < numMeshes;i++)
	{
		meshes[i] = BufferedAiMesh(aiMeshes[i], textures[i], colorMasks[i], textureLocation, colorLocation);
		//also store the verts here for collision stuff
		for (size_t j = 0;j < aiMeshes[i]->mNumVertices;j++)
		{
			verts.push_back(glm::vec4(aiMeshes[i]->mVertices[j].x, aiMeshes[i]->mVertices[j].y, aiMeshes[i]->mVertices[j].z, 1));
		}
	}
}

void Model::Draw()
{
	for (size_t i = 0;i < numMeshes;i++)
		meshes[i].Draw();
}