#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

Model::Model(std::string path, GLuint textureLocation, GLuint* textures, size_t numTextures)
{
	//load mesh
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	numMeshes = scene->mNumMeshes;
	//convert to buffered meshes
	aiMesh** aiMeshes = scene->mMeshes;
	meshes = new BufferedAiMesh[numMeshes];
	for (size_t i = 0;i < numMeshes;i++)
		meshes[i] = BufferedAiMesh(aiMeshes[i], textures[i], textureLocation);
}

void Model::Draw()
{
	for (size_t i = 0;i < numMeshes;i++)
		meshes[i].Draw();
}