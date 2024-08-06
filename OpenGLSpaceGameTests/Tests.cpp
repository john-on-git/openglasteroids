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
#include <GLFW/glfw3.h>

TEST(CubeAtOrigin, FaceCalculations) {
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
	
	GLuint
		textureLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "tex"),
		colorMaskLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "colorMask"),
		projectionLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "projection"),
		viewLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "view"),
		modelLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "model");
	
	auto shipTex = Texture("C:\\Users\\John\\source\\repos\\OpenGLSpaceGame\\OpenGLSpaceGame\\Textures\\ship.png");

	auto shipModel = Model(
		"C:\\Users\\John\\source\\repos\\OpenGLSpaceGame\\OpenGLSpaceGame\\Models\\uvmapped_cube.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ shipTex.handle },
		std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
		1
	);
	auto shipObject = WorldObject(
		&shipModel,
		glm::vec3(0.0f,  0.0f, -5.0f), //pos
		glm::vec3(0.0f,  0.0f,  0.0f), //rot
		glm::vec3(1.0f,  1.0f,  1.0f), //scale (1.0f = 2 units wide cube)
		projectionLocation,
		viewLocation,
		modelLocation,
		vector<tag>{ SHIP }
	);
	//there are twelve faces because each square "face" of the cube is split into two tris
	vector<glm::vec4> expectedFaces = {
		glm::vec4(0.0f,1.0f,0.0f,  1.0f), //+y (1)
		glm::vec4(0.0f,0.0f,1.0f,  1.0f), //+z (1)
		
		glm::vec4(-1.0f,0.0f,0.0f, 1.0f), //-x (2)
		glm::vec4(0.0f,-1.0f,0.0f, 1.0f), //-y (1)

		glm::vec4(1.0f,0.0f,0.0f,  1.0f), //+x (2)
		glm::vec4(0.0f,0.0f,-1.0f, 1.0f), //-z (1)

		glm::vec4(0.0f,1.0f,0.0f,  1.0f), //+y (2)
		glm::vec4(0.0f,0.0f,1.0f,  1.0f), //+z (2)

		glm::vec4(-1.0f,0.0f,0.0f, 1.0f), //-x (1)
		glm::vec4(0.0f,-1.0f,0.0f, 1.0f), //-y (2)

		glm::vec4(1.0f,0.0f,0.0f,  1.0f), //+x (1)
		glm::vec4(0.0f,0.0f,-1.0f, 1.0f), //-z (2)
	};
	auto actualFaces = shipObject.calcFaces(glm::vec3(0.0f, 0.0f, 0.0f));
	//string s = "";
	for (unsigned char i = 0; i < 12; i++)
	{
		/*s.append(std::to_string(i));
		s.append("\n\tExpected: ");
		s.append(std::to_string(expectedFaces.at(i)[0]));
		s.append(" ");
		s.append(std::to_string(expectedFaces.at(i)[1]));
		s.append(" ");
		s.append(std::to_string(expectedFaces.at(i)[2]));
		s.append(" ");
		s.append(std::to_string(expectedFaces.at(i)[3]));

		s.append("\n\t  Actual: ");
		s.append(std::to_string(actualFaces->at(i)[0]));
		s.append(" ");
		s.append(std::to_string(actualFaces->at(i)[1]));
		s.append(" ");
		s.append(std::to_string(actualFaces->at(i)[2]));
		s.append(" ");
		s.append(std::to_string(actualFaces->at(i)[3]));
		s.append("\n");*/

		EXPECT_EQ(expectedFaces.at(i), actualFaces->at(i));
		/*if (expectedFaces.at(i) == actualFaces->at(i))
		{
			s.append("TRUE");
		}
		else
		{
			s.append("FALSE");
		}
		s.append("\n");*/
	}
}
