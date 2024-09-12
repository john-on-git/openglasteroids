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
#include <ft2build.h>
#include FT_FREETYPE_H

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
#include "Renderer2D/Renderer2D.hpp"

using namespace std;

AppState* appState;
GLFWwindow* window;
bool keyPressed[360];

//AppState uses GoF State Pattern, with main.cpp as the Context
static void SetState(AppState* newState) {
	auto oldState = appState;
	//clear any GL state the old AppState left over
	glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(window);
	glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	appState = newState;
	delete oldState; //delete this after switching just in case deleting before causes any wacky behaviour
	appState->OnEntry();
}

static void GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::cerr << "GL Error: " << id << " " << message << std::endl;
}

int main()
{
	//initialize freetype
	FT_Library ftLibrary;
	FT_Face ftMainFont;
	FT_Error err;

	err = FT_Init_FreeType(&ftLibrary);
	if (err)
	{
		OutputDebugStringW(L"FATAL: failed to initialize freetype\n");
		exit(1);
	}
	err = FT_New_Face(ftLibrary, "C:/Windows/Fonts/times.ttf", 0, &ftMainFont);
	if (err)
	{
		OutputDebugStringW(L"FATAL: failed to load font (system does not support Times New Roman)\n");
		exit(1);
	}

	//(!ftMainFont->size)
	//FT_Set_Char_Size just calculates some values, then hands control to another function FT_Request_Size, which immediately errors if the above condition is false
	//no idea why, the purpose of the function is to init Face->size, why would you first check if it's already init?
	//docs: "When the FT_New_Face function is called (or one of its siblings), it automatically creates a new size object for the returned face. This size object is directly accessible as face−>size."
	//So I guess it's a bug?
	//it was bc the index in FT_New_Face was set to -1, docs say 0. -1 is for checking the number of faces
	
	err = FT_Set_Char_Size(
		ftMainFont,
		0, //docs say "Value of 0 for the character width means ‘same as character height’"
		FONT_CHAR_SIZE*64, //docs say it's measured in 1/64 of pixel, and recommend mult by 64
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);
	if (err)
	{
		OutputDebugStringW(L"FATAL: failed to init font\n");
		exit(1);
	}
	err = FT_Load_Glyph(
		ftMainFont,
		'A', //in ASCII
		FT_LOAD_DEFAULT
	);
	if (err)
	{
		OutputDebugStringW(L"FATAL: failed to init font\n");
		exit(1);
	}
	err = FT_Render_Glyph(ftMainFont->glyph, FT_RENDER_MODE_NORMAL);
	if (err)
	{
		OutputDebugStringW(L"FATAL: failed to init font\n");
		exit(1);
	}
	//TODO list of issues
	//	1. ISSUE: ftMainFont->glyph->bitmap.buffer is nullptr. CAUSE: was not calling FT_Render_Glyph
	//	2. ISSUE: crashes in Texture constructor. Probably because the bitmap is in the wrong format. CAUSE: old logic free'd the bitmap, causes a stack free bc it's no longer a pointer
	//	3. ISSUE: doesn't crash but nothing renders. Prob an issue with the vert shader, or rendering something really wacky bc of the bitmap format

	srand(time(NULL)); //initialize random

	//initialize glfw
	if (!glfwInit())
	{
		OutputDebugStringW(L"FATAL: failed to initialize glfw\n");
		exit(1);
	}
	//initialize window
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
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
	glad_glEnable(GL_DEBUG_OUTPUT);
	glad_glEnable(GL_DEPTH_TEST);
	glad_glEnable(GL_TEXTURE_2D);
	glad_glEnable(GL_BLEND);
	glad_glBlendEquation(GL_FUNC_ADD); // this is default
	glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//gl drawing config
	glad_glClearColor(0.0f, 0.0f, 1.0f, 1);
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
	Program textShader2D(
		"Shaders/TextShader2D/textShader2D.frag",
		"Shaders/TextShader2D/textShader2D.vert"
	);
	//get uniform locations
	textShader2D.Use();
	GLuint textureLocation2D = 0;//glad_glGetUniformLocation(textShader2D.handle, "tex");

	blockColorShader.Use();
	GLuint colorLocation = glad_glGetUniformLocation(blockColorShader.handle, "color");

	texturedColoredShader.Use();
	GLuint
		textureLocation	   = glad_glGetUniformLocation(texturedColoredShader.handle, "tex"),
		colorMaskLocation  = glad_glGetUniformLocation(texturedColoredShader.handle, "colorMask"),
		projectionLocation = glad_glGetUniformLocation(texturedColoredShader.handle, "projection"),
		modelViewLocation  = glad_glGetUniformLocation(texturedColoredShader.handle, "modelView");

	//set up projection matrix, put it on the GPU
	glad_glUniformMatrix4fv(
		projectionLocation,
		1,
		GL_FALSE,
		glm::value_ptr(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 10.0f)) //glm::value_ptr(glm::perspective(75.0f, 1.0f, 0.1f, 10.0f))
	);

	//check for failure
	if (projectionLocation == -1 || modelViewLocation == -1)
	{
		OutputDebugStringW(L"FATAL: couldn't get uniform location\n");
		exit(1);
	}

	//textures (from file)
	auto blankWhiteTex = Texture("textures/blankwhite.png");
	auto greeblingTex = Texture("textures/ship.png");
	auto cubeTex = Texture("textures/colored_johncat.bmp");

	//textures (font)
	auto newGameTextTex = Texture(ftMainFont->glyph->bitmap);

	//2d renderers
	auto newGameTextRenderer = Renderer2D(cubeTex.handle, textureLocation2D); //TODO

	//model
	auto asteroidModel = Model(
		"Models/cube.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ cubeTex.handle },
		std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
		1
	);
	auto projectileModel = Model(
		"Models/sphere.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ blankWhiteTex.handle },
		std::vector<glm::vec4>{ glm::vec4(2, 2, 2, 1) },
		1
	);
	auto planeModel = Model(
		"Models/plane.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ blankWhiteTex.handle },
		std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
		1
	);
	auto shipModel = Model(
		"Models/ship.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ blankWhiteTex.handle },
		std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
		1
	);

	std::map<std::string, Model*> models = {
		{"asteroid",&asteroidModel},
		{"projectile", &projectileModel},
		{"plane", &planeModel},
		{"ship", &shipModel}
	};

	std::map<std::string, Renderer2D*> renderer2ds = {
		{"newGameText",&newGameTextRenderer}
	};

	SetState(new MainMenu(SetState, keyPressed, &models, &renderer2ds, colorLocation, modelViewLocation, &texturedColoredShader, &blockColorShader, &textShader2D));
	while (!glfwWindowShouldClose(window)) //window
	{
		//clear framebuffers
		glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//check for button clicks
		appState->Tick();

		glad_glDebugMessageCallback(GLErrorCallback, nullptr);
		//check for GL errors
		/* TODO uncomment or delete
		GLenum err = glad_glGetError();
		while (err != GL_NO_ERROR)
		{
			cout << "\nGL Error: " << to_string(err) << endl;
			err = glad_glGetError();
		}*/
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