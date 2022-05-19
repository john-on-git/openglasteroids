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
#include "Class/myWorldThing/myWorldThing.h"
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

	glClearColor(1, 1, 1, 1); //set clear colour

	//set up polygons
	std::vector<MyWorldThing> worldThings;
	float verts1[] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f
	};
	worldThings.push_back(MyWorldThing(
		verts1,
		4,
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
	));

	float verts2[] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.2f, 0.0f, 0.0f
	};
	worldThings.push_back(MyWorldThing(
		verts2,
		4,
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
	));

	float verts3[] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f,
	};
	worldThings.push_back(MyWorldThing(
		verts3,
		3,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)
	));

	//init vertex buffer
	GLsizei vboSize = MyModel().totalVertices * sizeof(float[4]);

	GLuint VBO = 0;
	glad_glGenBuffers(vboSize, &VBO);
	glad_glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glad_glBufferData(GL_ARRAY_BUFFER, vboSize, NULL, GL_STATIC_DRAW); //internet said to do this

	for (auto& polygon : worldThings)
		polygon.model.MoveToBuffer();


	//vertex attrib array (default values? I guess this is just required?)
	//19.5.21 error was because I put this before VBO init!
	glad_glEnableVertexAttribArray(0);
	glad_glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

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

	/*char *bufferDataCheck = new char[vboSize];
	glad_glGetBufferSubData(GL_ARRAY_BUFFER, 0, vboSize, bufferDataCheck);
	for(int i=0;i<vboSize;i++)
	{
		printf("[");
		printf("%hhx", bufferDataCheck[i]);
		printf("] ");
	}*/

	while (!glfwWindowShouldClose(window))
	{
		glad_glClear(GL_COLOR_BUFFER_BIT); //clear the frame

		for (auto &thing : worldThings)
			thing.Draw(projectionLocation, viewLocation, modelLocation);

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