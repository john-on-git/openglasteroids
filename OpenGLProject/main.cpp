#include <iostream>
#include <fstream>
#include <windows.h>
#include <cmath>
#include <vector>
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

#include "Class/Program/program.hpp"
#include "Class/BufferedAiMesh/BufferedAiMesh.hpp"
#include "Class/WorldObject/WorldObject.hpp"
#include "Class/Texture/Texture.hpp"
#include "main.hpp"


using namespace std;

char moveX = 0, moveY = 0, moveZ = 0, rot = 0;
unsigned char axis = 2;

/*
	TODO
	texture support
	loading models from file
	licensing stuff for libs
*/
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
	//glEnables
		glad_glEnable(GL_DEPTH_TEST);
		glad_glEnable(GL_TEXTURE_2D);
	//gl drawing config
		glad_glClearColor(0.75f, 0.75f, 0.75f, 1);
		glad_glPointSize(5.0f);
	//shader setup
		//compile
			Program texturedTransform(
				"Shaders/TexturedTransform/texturedTransform.frag",
				"Shaders/TexturedTransform/texturedTransform.vert"
			);
		//get uniform locations
			texturedTransform.Use();
			GLuint
				textureLocation		= glad_glGetUniformLocation(texturedTransform.handle, "tex"),
				projectionLocation	= glad_glGetUniformLocation(texturedTransform.handle, "projection"),
				viewLocation		= glad_glGetUniformLocation(texturedTransform.handle, "view"),
				modelLocation		= glad_glGetUniformLocation(texturedTransform.handle, "model");
			//check for failure
			if (projectionLocation == -1 || viewLocation == -1 || modelLocation == -1)
			{
				OutputDebugStringW(L"FATAL: couldn't get uniform location\n");
				exit(1);
			}
	//textures
		auto shadow	= new Texture("textures/shadow.jpg");
		auto john	= new Texture("textures/colored_johncat.bmp");
		auto dish1	= new Texture("textures/dish_1.jpg");
		auto dish2	= new Texture("textures/dish_2.jpg");
	//set up world stuff
		vector<WorldObject*> worldThings{
			new WorldObject(
				new Model(
					"Models/dish.obj",
					textureLocation,
					new GLuint[]{ dish1->handle, dish2->handle },
					2
				),
				glm::vec3(  0.0f,   1.0f,  -5.0f),
				glm::vec3(180.0f,   0.0f,   0.0f),
				glm::vec3(  0.1f,   0.1f,   0.1f),
				projectionLocation,
				viewLocation,
				modelLocation
			),
			new WorldObject(
				new Model(
					"Models/utah_teapot_textured.obj",
					textureLocation,
					new GLuint[]{ john->handle },
					1
				),
				glm::vec3(  0.0f,   1.0f,  -5.0f),
				glm::vec3(  0.0f,   0.0f, 180.0f),
				glm::vec3(  0.2f,   0.2f,   0.2f),
				projectionLocation,
				viewLocation,
				modelLocation
			)
			/*,
			new MyWorldThing(
				new Model(
					"Models/uvmapped_cube.obj",
					textureLocation,
					new GLuint[]{ john->handle },
					1
				),
				glm::vec3( -1.0f,  -1.0f, -15.0f),
				glm::vec3(340.0f,  60.0f,   0.0f),
				glm::vec3(1.0f, 1.0f, 1.0f),
				projectionLocation,
				viewLocation,
				modelLocation
			)*/
		};
		WorldObject* first = worldThings[1];
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//print debug to console
			/*
			system("cls");
			cout << "triangle:"
				<< "\n\tposition"
					<< "\n\t\tx: "		<< first->position.x
					<< "\n\t\ty: "		<< first->position.y
					<< "\n\t\tz: "		<< first->position.z
				<< "\n\trotation: "
					<< "\n\t\tcur: "	<< (int)axis
					<< "\n\t\tx: "		<< first->angle.x
					<< "\n\t\ty: "		<< first->angle.y
					<< "\n\t\tz: "		<< first->angle.z
			;
			*/
		//update world state
			first->position.x += moveX * MOVE_RATE;
			first->position.y += moveY * MOVE_RATE;
			first->position.z += moveZ * MOVE_RATE;
			moveX = 0, moveY = 0, moveZ = 0;
			first->angle[axis] = (int)(first->angle[axis] + rot * ROTATE_RATE + 360) % 360;
			rot = 0;
		//clear framebuffers
			glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//draw all things
			for (auto& thing : worldThings)
				thing->Draw();
		//check for GL errors
			GLenum err = glad_glGetError();
			while(err!=GL_NO_ERROR)
			{
				cout << "\nGL Error: " << to_string(err);
				err = glad_glGetError();
			}
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
			moveY = -1;
		break;
		case GLFW_KEY_A:
			moveX = 1;
		break;
		case GLFW_KEY_S:
			moveY = 1;
		break;
		case GLFW_KEY_D:
			moveX = -1;
		break;
		case GLFW_KEY_R:
			moveZ = -1;
		break;
		case GLFW_KEY_F:
			moveZ = 1;
		break;
		case GLFW_KEY_Q:
			rot = -1;
		break;
		case GLFW_KEY_E:
			rot = 1;
		break;
		case GLFW_KEY_SPACE:
			++axis %= 3;
		break;
		default:
		break;
	}
}