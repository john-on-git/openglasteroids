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
	//initialize the bounding box with the values of an arbitrary vert from the model
	auto firstVert = scene->mMeshes[0]->mVertices[0];
	bBox = new glm::vec3[2]{
		glm::vec3(firstVert.x,firstVert.y,firstVert.z),
		glm::vec3(firstVert.x,firstVert.y,firstVert.z)
	}; //should this be on the heap?
	//calc bounding box. get the min and maximum x, y, & z coordinates of all vertices in the all meshes
	for (size_t i = 0;i < numMeshes;i++)
	{
		auto mesh = scene->mMeshes[i];
		for (size_t j = 1;j < mesh->mNumVertices;j++)
		{
			auto vert = mesh->mVertices[i];
			bBox[0].x = std::min(bBox[0].x, vert.x);
			bBox[1].x = std::max(bBox[1].x, vert.x);

			bBox[0].y = std::min(bBox[0].y, vert.y);
			bBox[1].y = std::max(bBox[1].y, vert.y);

			bBox[0].y = std::min(bBox[0].y, vert.y);
			bBox[1].y = std::max(bBox[1].y, vert.y);
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