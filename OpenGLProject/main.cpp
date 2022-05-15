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

#include "main.h"
#include "program.h"

using namespace std;


float verts[] = {
	 -0.5f,-0.5f,
	  0.0f, 0.5f,
	  0.5f,-0.5f
};
float point[] = { 0.5, 0.5 };

int main()
{
	//initialize glfw
	if (!glfwInit())
	{
		OutputDebugStringW(L"FATAL: failed to initialize glfw\n");
		return 1;
	}

	//initialize window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (window == NULL)
	{
		OutputDebugStringW(L"FATAL: failed to initialize glfw window\n");
		return 1;
	}
	glfwShowWindow(window);
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSetKeyCallback(window, key_callback);

	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		OutputDebugStringW(L"FATAL: failed to initialize glad\n");
		return 1;
	}

	//initialize OpenGL
	glClearColor(0, 0, 0, 1); //set clear colour
	//set up vertex buffer?
	GLuint VBO;
	glad_glGenBuffers(1, &VBO); //generate vertex buffer object
	glad_glBindBuffer(GL_ARRAY_BUFFER, VBO); //and bind 
	glad_glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(verts), verts, GL_STATIC_DRAW);

	//vertex attrib array (default values? I guess this is just required?) 
	glad_glEnableVertexAttribArray(0);
	glad_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//compile shaders (checking for failure)
	Program myFirstShader(
		"Shaders/MyFirstShader/myFirst.frag",
		"Shaders/MyFirstShader/myFirst.vert"
	);
	Program myFirstTransform(
		"Shaders/MyFirstTransform/myFirstTransform.frag",
		"Shaders/MyFirstTransform/myFirstTransform.vert"
	);


	//apply a translation transform
	myFirstTransform.use();
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate( //generate the matrix
		trans,
		glm::radians(90.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	GLint myFirstTransformLocation = glad_glGetUniformLocation(myFirstTransform.handle, "transform");
	if(myFirstTransformLocation==-1)
	{
		OutputDebugStringW(L"FATAL: couldn't get uniform handle\n");
		return 1;
	}
	glad_glUniformMatrix4fv( //pass it to the shader
		myFirstTransformLocation,
		1,
		GL_FALSE,
		glm::value_ptr(trans)
	);

	while (!glfwWindowShouldClose(window))
	{
		glad_glClear(GL_COLOR_BUFFER_BIT); //clear the frame

		//draw the first tri
		myFirstShader.use();
		glad_glDrawArrays(GL_TRIANGLES, 0, 3);


		glad_glUniformMatrix4fv( //pass it to the shader
			myFirstTransformLocation,
			1,
			GL_FALSE,
			glm::value_ptr(trans)
		);

		//spin the tri
		trans = glm::rotate( //generate the matrix
			trans,
			glm::radians(1.0f),
			glm::vec3(1.0f, 0.0f, 0.0f)
		);

		trans = glm::rotate( //generate the matrix
			trans,
			glm::radians(1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		//draw the second (transformed) tri
		myFirstTransform.use();
		glad_glUniformMatrix4fv( //pass the updated value to the shader
			myFirstTransformLocation,
			1,
			GL_FALSE,
			glm::value_ptr(trans)
		);
		glad_glDrawArrays(GL_TRIANGLES, 0, 3);

		//glfw boilerplate
		glfwSwapBuffers(window);
		glfwPollEvents();
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