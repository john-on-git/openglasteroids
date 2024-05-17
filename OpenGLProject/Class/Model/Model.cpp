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
	boundingBox = new glm::vec4[8]{
		glm::vec4(firstVert.x,firstVert.y,firstVert.z, 1.0f),
		glm::vec4(firstVert.x,firstVert.y,firstVert.z, 1.0f),

		glm::vec4(firstVert.x,firstVert.y,firstVert.z, 1.0f),
		glm::vec4(firstVert.x,firstVert.y,firstVert.z, 1.0f),

		glm::vec4(firstVert.x,firstVert.y,firstVert.z, 1.0f),
		glm::vec4(firstVert.x,firstVert.y,firstVert.z, 1.0f),

		glm::vec4(firstVert.x,firstVert.y,firstVert.z, 1.0f),
		glm::vec4(firstVert.x,firstVert.y,firstVert.z, 1.0f),
	};
	//convert to buffered meshes
	aiMesh** aiMeshes = scene->mMeshes;
	meshes = new BufferedAiMesh[numMeshes];
	for (size_t i = 0;i < numMeshes;i++)
	{
		meshes[i] = BufferedAiMesh(aiMeshes[i], textures[i], colorMasks[i], textureLocation, colorLocation);
		//also store the verts here for collision stuff
		for (size_t j = 0;j < aiMeshes[i]->mNumVertices;j++)
		{
			//also update the bounding box
			auto vert = aiMeshes[i]->mVertices[j];
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
	}
}

void Model::Draw()
{
	for (size_t i = 0;i < numMeshes;i++)
		meshes[i].Draw();
}