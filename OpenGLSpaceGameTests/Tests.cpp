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
#include <Program/program.hpp>
#include <Program/program.cpp>
#include <Texture/Texture.cpp>
#include <Texture/Texture.hpp>
#include <QuadTreeCollisionHandler/QuadTreeCollisionHandler.hpp>
#include <QuadTreeCollisionHandler/QuadTreeCollisionHandler.cpp>
#include <GLFW/glfw3.h>

namespace Tests {

class CollisionDetectionTests : public testing::Test {
protected:
	CollisionDetectionTests()
	{
		//initialize glfw
		EXPECT_TRUE(glfwInit());

		//initialize window
		GLFWwindow* window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
		EXPECT_FALSE(window == nullptr);
		glfwHideWindow(window);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);

		//initialize glad
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			OutputDebugStringW(L"FATAL: failed to initialize glad\n");
			exit(1);
		}
		Program texturedColoredShader(
			"Shaders/TexturedColoredTransform/texturedColoredTransform.frag",
			"Shaders/TexturedColoredTransform/texturedColoredTransform.vert"
		);

		GLuint textureLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "tex");
		GLuint colorMaskLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "colorMask");
		projectionLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "projection");
		viewLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "view");
		modelLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "model");

		Texture shipTex = Texture("C:\\Users\\John\\source\\repos\\OpenGLSpaceGame\\OpenGLSpaceGame\\Textures\\ship.png");

		cubeModel = new Model(
			"C:\\Users\\John\\source\\repos\\OpenGLSpaceGame\\OpenGLSpaceGame\\Models\\cube.obj",
			textureLocation,
			colorMaskLocation,
			std::vector<GLuint>{ shipTex.handle },
			std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
			1
		);
		shipModel = new Model(
			"C:\\Users\\John\\source\\repos\\OpenGLSpaceGame\\OpenGLSpaceGame\\Models\\ship.obj",
			textureLocation,
			colorMaskLocation,
			std::vector<GLuint>{ shipTex.handle },
			std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
			1
		);
	}
	Model* cubeModel;
	Model* shipModel;
	GLuint projectionLocation, viewLocation, modelLocation;
};

//that the correct face data is generated for a basic object
TEST_F(CollisionDetectionTests, CorrectFacesForCubeAtOrigin) {
	auto cube = WorldObject(
		cubeModel,
		glm::vec3(0.0f, 0.0f, 0.0f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		projectionLocation,
		viewLocation,
		modelLocation,
		vector<tag>{ }
	);
	//there are twelve faces because each square "face" of the cube is split into two tris
	vector<glm::vec4> expectedFaces = {
		glm::vec4(0.0f,-1.0f,0.0f,  1.0f), //-y (1)
		glm::vec4(0.0f,0.0f,-1.0f,  1.0f), //-z (1)

		glm::vec4(1.0f,0.0f,0.0f,   1.0f), //+x (1)
		glm::vec4(0.0f,1.0f,0.0f,   1.0f), //+y (1)

		glm::vec4(-1.0f,0.0f,0.0f,  1.0f), //-x (1)
		glm::vec4(0.0f,0.0f,1.0f,  1.0f), //+z (1)

		glm::vec4(0.0f,-1.0f, 0.0f, 1.0f), //-y (2)
		glm::vec4(0.0f, 0.0f,-1.0f, 1.0f), //-z (2)

		glm::vec4(1.0f,0.0f,0.0f,   1.0f), //+x (2)
		glm::vec4(0.0f,1.0f,0.0f,   1.0f), //+y (2)

		glm::vec4(-1.0f,0.0f,0.0f,  1.0f), //-x (2)
		glm::vec4(0.0f,0.0f,1.0f,  1.0f), //+z (2)
	};
	auto actualFaces = cube.calcFaces();
	for (unsigned char i = 0; i < 12; i++)
	{
		EXPECT_EQ(expectedFaces.at(i), actualFaces->at(i));
	}
}

//that the collision is correctly detected for simple objects in a few different positions
//expected colliding
TEST_F(CollisionDetectionTests, CubesIntersectingNearOrigin) {
	auto firstCube = WorldObject(
		cubeModel,
		glm::vec3(0.0f, 0.0f, 0.0f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		projectionLocation,
		viewLocation,
		modelLocation,
		vector<tag>{ }
	);
	auto secondCube = WorldObject(
		cubeModel,
		glm::vec3(1.9f, 0.1f, 0.1f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		projectionLocation,
		viewLocation,
		modelLocation,
		vector<tag>{ }
	);
	QuadTreeCollisionHandler collisionHandler(
		10,
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);
	collisionHandler.Register(&firstCube);
	collisionHandler.Register(&secondCube);
	auto collisions = collisionHandler.Check();
	EXPECT_EQ(collisions->size(), 1);
	for (auto it = collisions->begin(); it != collisions->end(); it++) //for each collision
	{
		EXPECT_TRUE(
			(it->first==&firstCube && it->second== &secondCube) ||
			(it->first== &secondCube && it->second== &firstCube)
		);
	}
	delete collisions;
}

TEST_F(CollisionDetectionTests, CubesIntersectingFarFromOrigin) {
	auto firstCube = WorldObject(
		cubeModel,
		glm::vec3(0.0f + 100.1f, 0.0f + 100.1f, 0.0f + 100.1f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		projectionLocation,
		viewLocation,
		modelLocation,
		vector<tag>{ }
	);
	auto secondCube = WorldObject(
		cubeModel,
		glm::vec3(1.9f + 100.1f, 0.1f + 100.1f, 0.1f + 100.1f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		projectionLocation,
		viewLocation,
		modelLocation,
		vector<tag>{ }
	);
	QuadTreeCollisionHandler collisionHandler(
		10,
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);
	collisionHandler.Register(&firstCube);
	collisionHandler.Register(&secondCube);
	auto collisions = collisionHandler.Check();
	EXPECT_EQ(collisions->size(), 1);
	for (auto it = collisions->begin(); it != collisions->end(); it++) //for each collision
	{
		EXPECT_TRUE(
			(it->first == &firstCube && it->second == &secondCube) ||
			(it->first == &secondCube && it->second == &firstCube)
		);
	}
	delete collisions;
}

//expected not colliding

TEST_F(CollisionDetectionTests, CubesNonintersectingButCloseNearOrigin) {
	SUCCEED() << "Not Implemented";
}

TEST_F(CollisionDetectionTests, CubesNonintersectingNearOrigin) {
	SUCCEED() << "Not Implemented";
}

TEST_F(CollisionDetectionTests, CubesNonintersectingFarFromOrigin) {
	SUCCEED() << "Not Implemented";
}

//TODO the collision output depends on the spin of the object (spotted using ship and cube)
//write some tests to determine what's going on
//it's not scale related
//it's some combination of angle and distance
//it's related to distance from the origin!

/*
False Positive
	0:
		Model: ship
		scale    = {x=1            y=1           z=1}
		position = {x=-0.984208584 y=-1.37918746 z=-15.0}
		angle    = {x=270.000000   y=316.398773  z=0}
	1:
		Model: cube
		scale    = {x=1            y=1           z=1}
		position = {x=0.400000006  y=0.400000006 z=-15}
		angle    = {x=270.000000   y=0           z=0}
*/

TEST_F(CollisionDetectionTests, ManuallyFoundFalsePositive) {
	auto ship = WorldObject(
		shipModel,
		glm::vec3(-0.984208584f, -1.37918746f, -15.0f), //pos
		glm::vec3(270.0f, 316.4f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		projectionLocation,
		viewLocation,
		modelLocation,
		vector<tag>{ SHIP }
	);
	auto cube = WorldObject(
		cubeModel,
		glm::vec3(0.4f,   0.4f, -15.0f), //pos
		glm::vec3(270.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f,   1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		projectionLocation,
		viewLocation,
		modelLocation,
		vector<tag>{ }
	);
	QuadTreeCollisionHandler collisionHandler(
		10,
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);
	collisionHandler.Register(&ship);
	collisionHandler.Register(&cube);
	auto collisions = collisionHandler.Check();
	EXPECT_EQ(collisions->size(), 0);
	delete collisions;
}

} //Tests 