//std
	#include <iostream>
	#include <fstream>
	#include <windows.h>
	#include <cmath>
	#include <vector>
//opengl and libs
	#include <glad/glad.h>
	#include <GLFW/glfw3.h>
//glm
	#include <glm/vec3.hpp>
	#include <glm/vec4.hpp>
	#include <glm/mat4x4.hpp>
	#include <glm/ext/matrix_transform.hpp>
	#include <glm/ext/matrix_clip_space.hpp>
	#include <glm/ext/scalar_constants.hpp>
	#include <glm/glm.hpp>
	#include <glm/gtc/type_ptr.hpp>
//project
	#include "Class/Program/program.h"
	#include "Class/myWorldThing/myWorldThing.h"
	#include "main.h"

using namespace std;

char moveX = 0, moveY = 0, moveZ = 0;

int main()
{
	//initialize glfw
		if (!glfwInit())
		{
			OutputDebugStringW(L"FATAL: failed to initialize glfw\n");
			exit(1);
		}
	//initialize window
		GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
		if (window == NULL)
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
	//set clear colour
		glClearColor(1, 1, 1, 1);
	//set up VAO(s)
		GLuint VAOs[1];
		glad_glGenVertexArrays(1, VAOs);
		glad_glBindVertexArray(VAOs[0]);
	//set up world stuff
		glad_glPointSize(10);
		std::vector<MyWorldThing> worldThings;
		MyWorldThing cube(
			new float[] { //cube
				0.0f, 0.0f, 0.0f, 0.0f,
				0.5f, 0.0f, 0.0f, 0.0f,
				0.5f, 0.5f, 0.0f, 0.0f,
				0.0f, 0.5f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
				0.5f, 0.0f, 0.0f, 1.0f,
				0.5f, 0.5f, 0.0f, 1.0f,
				0.0f, 0.5f, 0.0f, 1.0f
			},
			32,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			VAOs[0]
		);
		//worldThings.push_back(cube);
		MyWorldThing triangle(
			new float[] {
				0.0f, 0.0f, 0.0f, 1.0f,
				0.2f, 0.0f, 0.0f, 1.0f,
				0.1f, 0.1f, 0.0f, 1.0f
			},
			12,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			VAOs[0]
		);
		worldThings.push_back(triangle);
	//shader setup
		//compile
			Program myFirstTransform(
				"Shaders/MyFirstTransform/myFirstTransform.frag",
				"Shaders/MyFirstTransform/myFirstTransform.vert"
			);
		//get uniform locations
			myFirstTransform.Use();
			GLint
				projectionLocation	= glad_glGetUniformLocation(myFirstTransform.handle, "projection"),
				viewLocation		= glad_glGetUniformLocation(myFirstTransform.handle, "view"),
				modelLocation		= glad_glGetUniformLocation(myFirstTransform.handle, "model");
			//check for failure
			if (projectionLocation == -1 || viewLocation == -1 || modelLocation == -1)
			{
				OutputDebugStringW(L"FATAL: couldn't get uniform location\n");
				exit(1);
			}
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//update world state
			cube.position.x += moveX;
			cube.position.y += moveY;
			cube.position.z += moveZ;
			triangle.position.x += moveX;
			triangle.position.y += moveY;
			triangle.position.z += moveZ;
			moveX = 0;
			moveY = 0;
			moveZ = 0;
		//clear framebuffers
			glad_glClear(GL_COLOR_BUFFER_BIT);
			glad_glClear(GL_DEPTH_BUFFER_BIT);
		//draw all objects
			for (auto& thing : worldThings)
				thing.Draw(projectionLocation, viewLocation, modelLocation);
		//glfw stuff
			glfwSwapBuffers(window);
			glfwPollEvents();
		Sleep(FRAME_DELAY);
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
	switch (key)
	{
		case GLFW_KEY_W:
			moveX = 1;
		break;
		case GLFW_KEY_A:
			moveX = -1;
		break;
		case GLFW_KEY_S:
			moveY = 1;
		break;
		case GLFW_KEY_D:
			moveY = -1;
		break;
		case GLFW_KEY_Q:
			moveZ = 1;
			break;
		case GLFW_KEY_E:
			moveZ = -1;
		break;
		default:
		break;
	}
}