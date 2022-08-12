#include "Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

Model::Model(std::string path, GLuint textureLocation, GLuint colorLocation, GLuint* textures, glm::vec4* colorMasks, size_t numTextures)
{
	//load mesh
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	numMeshes = scene->mNumMeshes;
	//calc bounding box. get the min and maximum x, y, & z coordinates of all vertices in the all meshes
	for (size_t i = 0;i < numMeshes;i++)
	{
		auto mesh = scene->mMeshes[i];
		for (size_t j = 0;j < mesh->mNumVertices;j++)
		{
			auto vert = mesh->mVertices[i];
			if (bBox[0].x < vert.x)
				bBox[0].x = vert.x;
			else if (bBox[1].x > vert.x)
				bBox[1].x = vert.x;

			if (bBox[0].y < vert.y)
				bBox[0].y = vert.y;
			else if (bBox[1].y > vert.y)
				bBox[1].y = vert.y;

			if (bBox[0].z < vert.z)
				bBox[0].z = vert.z;
			else if (bBox[1].z > vert.z)
				bBox[1].z = vert.z;
		}
	}
	//convert to buffered meshes
	aiMesh** aiMeshes = scene->mMeshes;
	meshes = new BufferedAiMesh[numMeshes];
	for (size_t i = 0;i < numMeshes;i++)
		meshes[i] = BufferedAiMesh(aiMeshes[i], textures[i], colorMasks[i], textureLocation, colorLocation);
}

void Model::Draw()
{
	for (size_t i = 0;i < numMeshes;i++)
		meshes[i].Draw();
}