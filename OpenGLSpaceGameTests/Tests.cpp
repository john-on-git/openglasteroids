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


//TODO the collision output depends on the spin of the object (spotted using ship and cube)
//write some tests to determine what's going on
//it's not scale related
//it's some combination of angle and distance
//it's related to distance from the origin! 
//It's actually an issue with broad collisions :skull: no wait it's actually an issue with both??? No. It's just with fine, broad broke because it was outside the quadtree (it should prob auto-expand or smth)

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
		auto colorMaskLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "colorMask");
		auto projectionLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "projection");
		modelViewLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "modelView");

		Texture noiseTex = Texture("C:\\Users\\John\\source\\repos\\OpenGLSpaceGame\\OpenGLSpaceGameTests\\Textures\\noise.png");

		auto a = "C:\\Users\\John\\source\\repos\\OpenGLSpaceGame\\OpenGLSpaceGameTests";
		auto b = "C:\\Users\\John\\source\\repos\\OpenGLSpaceGame\\OpenGLSpaceGameTests\\Models\\cube.obj";

		cubeModel = new Model(
			"C:\\Users\\John\\source\\repos\\OpenGLSpaceGame\\OpenGLSpaceGameTests\\Models\\cube.obj",
			textureLocation,
			colorMaskLocation,
			std::vector<GLuint>{ noiseTex.handle },
			1
		);
		shipModel = new Model(
			"C:\\Users\\John\\source\\repos\\OpenGLSpaceGame\\OpenGLSpaceGameTests\\Models\\ship.obj",
			textureLocation,
			colorMaskLocation,
			std::vector<GLuint>{ noiseTex.handle },
			1
		);
	}
	Model* cubeModel;
	Model* shipModel;
	GLuint modelViewLocation;
};

//that the correct face data is generated for a basic object
TEST_F(CollisionDetectionTests, CorrectFacesForCubeAtOrigin) {
	auto cube = WorldObject(
		cubeModel,
		glm::vec3(0.0f, 0.0f, 0.0f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		modelViewLocation,
		unordered_set<tag>{ }
	);
	//there are twelve faces because each square "face" of the cube is split into two tris
	constexpr size_t EXPECTED_NUM_FACES = 12;
	glm::vec4 expectedFaces[EXPECTED_NUM_FACES] = {
		glm::vec4(0.0f,-1.0f,0.0f,  -1.0f), //-y (1)
		glm::vec4(0.0f,0.0f,-1.0f,  -1.0f), //-z (1)

		glm::vec4(1.0f,0.0f,0.0f,   -1.0f), //+x (1)
		glm::vec4(0.0f,1.0f,0.0f,   -1.0f), //+y (1)

		glm::vec4(-1.0f,0.0f,0.0f,  -1.0f), //-x (1)
		glm::vec4(0.0f,0.0f,1.0f,  -1.0f), //+z (1)

		glm::vec4(0.0f,-1.0f, 0.0f, -1.0f), //-y (2)
		glm::vec4(0.0f, 0.0f,-1.0f, -1.0f), //-z (2)

		glm::vec4(1.0f,0.0f,0.0f,   -1.0f), //+x (2)
		glm::vec4(0.0f,1.0f,0.0f,   -1.0f), //+y (2)

		glm::vec4(-1.0f,0.0f,0.0f,  -1.0f), //-x (2)
		glm::vec4(0.0f,0.0f,1.0f,  -1.0f), //+z (2)
	};
	glm::vec4* actualFaces = cube.calcFaces();
	
	size_t matches = 0;
	for (size_t i=0;i<EXPECTED_NUM_FACES;i++)
	{
		//i = 9, j = 2
		//remove this face if it was 
		for (size_t j = 0; j < EXPECTED_NUM_FACES; j++)
		{
			if (expectedFaces[i] == actualFaces[j]) {
				expectedFaces[i] = glm::vec4(0, 0, 0, 0);
				matches++;
				break;
			}
		}
	}
	//expect all expected faces to have been removed (because they were all in the actual faces)
	EXPECT_EQ(matches,12);
	delete[] actualFaces;
}

//that the collision is correctly detected for simple objects in a few different positions
//expected colliding
TEST_F(CollisionDetectionTests, CubesIntersectingNearOrigin) {
	auto firstCube = WorldObject(
		cubeModel,
		glm::vec3(0.0f, 0.0f, 0.0f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		modelViewLocation,
		unordered_set<tag>{ }
	);
	auto secondCube = WorldObject(
		cubeModel,
		glm::vec3(1.9f, 0.1f, 0.1f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		modelViewLocation,
		unordered_set<tag>{ }
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
		glm::vec3(0.0f + 20.0f, 0.0f + 20.0f, 0.0f + 20.0f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		modelViewLocation,
		unordered_set<tag>{ }
	);
	auto secondCube = WorldObject(
		cubeModel,
		glm::vec3(1.9f + 20.0f, 0.1f + 20.0f, 0.1f + 20.0f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		modelViewLocation,
		unordered_set<tag>{ }
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

	//e1f1 parallel outside
	//e1f2 back-facing

	//there's something wrong with the plane normal. it's inverted.
	//this was my first guess :skull:
	//this answers my question, and that's why the w had to be inverted?
	//error in face calculations line 179 (a-b) instead of (b-a), so it was backwards, remember your right hand rule
}

//expected not colliding

TEST_F(CollisionDetectionTests, CubesNonintersectingNearOrigin) {
	auto firstCube = WorldObject(
		cubeModel,
		glm::vec3(0.0f, 0.0f, 0.0f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		modelViewLocation,
		unordered_set<tag>{ }
	);
	auto secondCube = WorldObject(
		cubeModel,
		glm::vec3(2.001f, 0.1f, 0.1f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		modelViewLocation,
		unordered_set<tag>{ }
	);
	QuadTreeCollisionHandler collisionHandler(
		10,
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);
	collisionHandler.Register(&firstCube);
	collisionHandler.Register(&secondCube);
	auto collisions = collisionHandler.Check();
	EXPECT_EQ(collisions->size(), 0);
	delete collisions;
}
TEST_F(CollisionDetectionTests, CubesNonintersectingFarFromOrigin) {
	auto firstCube = WorldObject(
		cubeModel,
		glm::vec3(0.0f + 20.0f, 0.0f + 20.0f, 0.0f + 20.0f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		modelViewLocation,
		unordered_set<tag>{ }
	);
	auto secondCube = WorldObject(
		cubeModel,
		glm::vec3(2.001f + 20.0f, 0.1f + 20.0f, 0.1f + 20.0f), //pos
		glm::vec3(0.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f, 1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		modelViewLocation,
		unordered_set<tag>{ }
	);
	QuadTreeCollisionHandler collisionHandler(
		10,
		glm::vec2(-100.0f, -100.0f),
		glm::vec2(100.0f, 100.0f)
	);
	collisionHandler.Register(&firstCube);
	collisionHandler.Register(&secondCube);
	auto collisions = collisionHandler.Check();
	EXPECT_EQ(collisions->size(), 0);
	delete collisions;
}

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
		modelViewLocation,
		unordered_set<tag>{ SHIP }
	);
	auto cube = WorldObject(
		cubeModel,
		glm::vec3(0.4f,   0.4f, -15.0f), //pos
		glm::vec3(270.0f, 0.0f, 0.0f), //rot
		glm::vec3(1.0f,   1.0f, 1.0f), //scale (1.0f = 2 units wide cube)
		modelViewLocation,
		unordered_set<tag>{ }
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