#include <gtest/gtest.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <algorithm>

//these work because the OpenGLSpaceGame/Source is listen in the additonal in this project's additional include directiories
#include <Model/Model.hpp>
#include <Model/Model.cpp>
#include <WorldObject/WorldObject.hpp>
#include <WorldObject/WorldObject.cpp>
#include <BufferedAiMesh/BufferedAiMesh.cpp>

TEST(TestCaseName, TestName) {
	auto shipModel = Model(
		"Models/uvmapped_cube.obj",
		NULL,
		NULL,
		std::vector<GLuint>{ NULL },
		std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
		1
	);
	auto shipObject = WorldObject(
		&shipModel,
		glm::vec3(0.0f,  0.0f, -5.0f), //pos
		glm::vec3(0.0f,  0.0f,  0.0f), //rot
		glm::vec3(1.0f,  1.0f,  1.0f), //scale
		NULL,
		NULL,
		NULL,
		vector<tag>{ SHIP }
	);
	auto actualFaces = shipObject.calcFaces(glm::vec3(0.0f, 0.0f, 0.0f));
	vector<glm::vec4> expectedFaces = {
		glm::vec4( 1.0f,0.0f,0.0f,0.0f), glm::vec4( 1.0f,0.0f,0.0f,0.0f), //+x
		glm::vec4(-1.0f,0.0f,0.0f,0.0f), glm::vec4(-1.0f,0.0f,0.0f,0.0f), //-x

		glm::vec4(0.0f, 1.0f,0.0f,0.0f), glm::vec4(0.0f, 1.0f,0.0f,0.0f), //+y
		glm::vec4(0.0f,-1.0f,0.0f,0.0f), glm::vec4(0.0f,-1.0f,0.0f,0.0f), //-y

		glm::vec4(0.0f,0.0f, 1.0f,0.0f), glm::vec4(0.0f,0.0f, 1.0f,0.0f), //+z
		glm::vec4(0.0f,0.0f,-1.0f,0.0f), glm::vec4(0.0f,0.0f,-1.0f,0.0f), //-z
	};
	//std::sort(actualFaces->begin(), actualFaces->end());
	//std::sort(expectedFaces.begin(), expectedFaces.end());
	for (unsigned char i = 0; i < 12; i++)
	{
		//EXPECT_TRUE(glm::vec3() == glm::vec3());
		//EXPECT_TRUE(glm::equal(expectedFaces.at(i), actualFaces->at(i)));
	}
}