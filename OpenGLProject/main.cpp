//std
#include <iostream>
#include <fstream>
#include <windows.h>
#include <cmath>
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

#include "Class/Program/program.h"
#include "Class/MyPolygon/myPolygon.h"
#include "main.h"
#include <vector>

using namespace std;

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
	glfwShowWindow(window);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		OutputDebugStringW(L"FATAL: failed to initialize glad\n");
		exit(1);
	}

	//initialize OpenGL
	gladLoadGL();

	glClearColor(1, 1, 1, 1); //set clear colour

	//set up polygons
	std::vector<MyPolygon> polygons;
	float verts1[] = {
		0.0f,  0.0f,
		0.5f,  0.0f,
		0.5f,  0.5f,
		0.0f,  0.5f
	};
	polygons.push_back(MyPolygon(
		verts1,
		8,
		glm::vec2(-0.5f, -0.5f),
		0.0f
	));

	float verts2[] = {
		0.0f,  0.0f,
		0.5f,  0.0f,
		0.5f,  0.5f,
		0.0f,  0.2f
	};
	polygons.push_back(MyPolygon(
		verts2,
		8,
		glm::vec2(0.5f, 0.5f),
		0.0f
	));

	float verts3[] = {
		0.0f,  0.0f,
		0.5f,  0.0f,
		0.5f,  0.5f
	};
	polygons.push_back(MyPolygon(
		verts3,
		6,
		glm::vec2(0.0f, 0.0f),
		0.0f
	));

	//vertex attrib array (default values? I guess this is just required?) 
	glad_glEnableVertexAttribArray(0);
	glad_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//compile shader(s), checking for failure
	Program myFirstTransform(
		"Shaders/MyFirstTransform/myFirstTransform.frag",
		"Shaders/MyFirstTransform/myFirstTransform.vert"
	);

	//use the transformation shader, ready to insert values into the uniform
	myFirstTransform.Use();
	GLint projectionLocation = glad_glGetUniformLocation(myFirstTransform.handle, "projection");
	if(projectionLocation ==-1)
	{
		OutputDebugStringW(L"FATAL: couldn't get uniform handle\n");
		exit(1);
	}

	GLint viewLocation = glad_glGetUniformLocation(myFirstTransform.handle, "view");
	if (viewLocation == -1)
	{
		OutputDebugStringW(L"FATAL: couldn't get uniform handle\n");
		exit(1);
	}

	GLint modelLocation = glad_glGetUniformLocation(myFirstTransform.handle, "model");
	if (modelLocation == -1)
	{
		OutputDebugStringW(L"FATAL: couldn't get uniform handle\n");
		exit(1);
	}

	while (!glfwWindowShouldClose(window))
	{
		glad_glClear(GL_COLOR_BUFFER_BIT); //clear the frame

		for (auto &polygon : polygons)
			polygon.Draw(projectionLocation, viewLocation, modelLocation);
		

		//glfw boilerplate
		glfwSwapBuffers(window);
		glfwPollEvents();

		Sleep(FRAME_DELAY);
	}
	
	//free resources
	glfwTerminate(); //all glfw resources
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
		default:
		break;
	}
}