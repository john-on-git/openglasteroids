#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <windows.h>
#include <cmath>
#include <vector>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <conio.h>
#include <assimp/Importer.hpp>
#include <assimp\postprocess.h>

#include "Program/program.hpp"
#include "BufferedAiMesh/BufferedAiMesh.hpp"
#include "WorldObject/SpaceGameObject.hpp"
#include "WorldObject/TemporarySpaceGameObject.hpp"
#include "Texture/Texture.hpp"
#include "main.hpp"
#include "QuadTreeCollisionHandler/QuadTreeCollisionHandler.hpp"

#include "AppState/AppState.hpp"
#include "AppState/MainMenu.hpp"
#include "AppState/GameInProgress.hpp"

using namespace std;

AppState* appState;
bool keyPressed[360];

//AppState uses GoF State Pattern, with main.cpp as the Context
static void SetState(AppState* newState) {
	delete appState;
	appState = newState;
}


int main()
{
	srand(time(NULL));
	//initialize glfw
	if (!glfwInit())
	{
		OutputDebugStringW(L"FATAL: failed to initialize glfw\n");
		exit(1);
	}
	//initialize window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	if (window == nullptr)
	{
		OutputDebugStringW(L"FATAL: failed to initialize glfw window\n");
		exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1);
	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		OutputDebugStringW(L"FATAL: failed to initialize glad\n");
		exit(1);
	}
	//glEnables
	glad_glEnable(GL_DEPTH_TEST);
	glad_glEnable(GL_TEXTURE_2D);
	glad_glEnable(GL_BLEND);
	glad_glBlendEquation(GL_FUNC_ADD); // this is default
	glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//gl drawing config
	glad_glClearColor(0.0f, 0.0f, 0.0f, 1);
	glad_glPointSize(5.0f);

	//shader setup
	//compile
	Program texturedColoredShader(
		"Shaders/TexturedColoredTransform/texturedColoredTransform.frag",
		"Shaders/TexturedColoredTransform/texturedColoredTransform.vert"
	);
	Program blockColorShader(
		"Shaders/BlockColor/blockColor.frag",
		"Shaders/BlockColor/blockColor.vert"
	);
	//get uniform locations
	blockColorShader.Use();
	GLuint colorLocation = glad_glGetUniformLocation(blockColorShader.handle, "color");

	texturedColoredShader.Use();
	GLuint
		textureLocation		= glad_glGetUniformLocation(texturedColoredShader.handle, "tex"),
		colorMaskLocation	= glad_glGetUniformLocation(texturedColoredShader.handle, "colorMask"),
		projectionLocation	= glad_glGetUniformLocation(texturedColoredShader.handle, "projection"),
		viewLocation		= glad_glGetUniformLocation(texturedColoredShader.handle, "view"),
		modelLocation		= glad_glGetUniformLocation(texturedColoredShader.handle, "model");
	//check for failure
	if (projectionLocation == -1 || viewLocation == -1 || modelLocation == -1)
	{
		OutputDebugStringW(L"FATAL: couldn't get uniform location\n");
		exit(1);
	}

	//textures
	auto shipTex = Texture("textures/blankwhite.png");
	auto projectileTex = Texture("textures/blankwhite.png");
	auto graniteTex = Texture("textures/wikimedia_Pink_granite_tileable_1024x1024.png");

	//model
	auto shipModel = Model(
		"Models/ship.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ shipTex.handle },
		std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
		1
	);
	auto projectileModel = Model(
		"Models/sphere.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ projectileTex.handle },
		std::vector<glm::vec4>{ glm::vec4(2, 2, 2, 1) },
		1
	);
	auto asteroidModel = Model(
		"Models/cube.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ graniteTex.handle },
		std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
		1
	);

	appState = new MainMenu(SetState, keyPressed, &asteroidModel, &projectileModel, &shipModel, colorLocation, modelLocation, projectionLocation, viewLocation, &texturedColoredShader, &blockColorShader);

	while (!glfwWindowShouldClose(window)) //window
	{
		//check for button clicks
		appState->Tick();

		//cerr << "High scores not implemented" << endl;

		//check for GL errors
		GLenum err = glad_glGetError();
		while (err != GL_NO_ERROR)
		{
			cout << "\nGL Error: " << to_string(err);
			err = glad_glGetError();
			err = glad_glGetError();
		}
		//glfw stuff
		glfwSwapBuffers(window);
		glfwPollEvents();

		Sleep(FRAME_DELAY); //FPS cap
	}

	//free resources
		//glfw
		glfwDestroyWindow(window);
		glfwTerminate();
		//vertex buffer
		//vertex array
}

/// <summary>
/// called whenever a key is pressed
/// </summary>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keyPressed[key] = true;
	}
	else if (action==GLFW_RELEASE)
	{
		keyPressed[key] = false;
	}
}