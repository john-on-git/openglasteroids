#define _USE_MATH_DEFINES

#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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
#include <freetype/ftbitmap.h>


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
#include "TextBox/TextBox.hpp"

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

/// <summary>
/// called whenever a key is pressed
/// </summary>
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keyPressed[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		keyPressed[key] = false;
	}
}

int main()
{
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
	glad_glClearColor(0.1f, 0.1f, 0.1f, 1);
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
	GLuint 
		textureLocation2D = glad_glGetUniformLocation(textShader2D.handle, "tex"),
		translationLocation2D = glad_glGetUniformLocation(textShader2D.handle, "translation");

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
		glm::value_ptr(glm::perspective(75.0f, 1.0f, 0.1f, 10.0f))
	);

	//check for failure
	if (projectionLocation == -1 || modelViewLocation == -1)
	{
		OutputDebugStringW(L"FATAL: couldn't get uniform location\n");
		exit(1);
	}


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

	FT_Set_Char_Size(
		ftMainFont,
		FONT_RESOLUTION * 64, //docs say "Value of 0 for the character width means 'same as character height'"
		0, //docs say it's measured in 1/64 of pixel, and recommend mult by 64
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);

	size_t charAtlasWidth = 0;
	size_t charAtlasRows = 0;
	size_t widthPerCharacter = 0;
	vector<FT_Bitmap> charBitmapsTemp = vector<FT_Bitmap>();

	//build the texture atlas
	for (char i = 'A'; i <= 'Z'; i++)
	{
		FT_Load_Char(
			ftMainFont,
			i, //in ASCII
			FT_LOAD_DEFAULT
		);
		FT_Render_Glyph(ftMainFont->glyph, FT_RENDER_MODE_NORMAL);

		//take a new copy of the bitmap, just to ensure that rendering characters isn't overwriting the old ones
		FT_Bitmap temp = FT_Bitmap();
		temp.width = ftMainFont->glyph->bitmap.width;
		temp.rows = ftMainFont->glyph->bitmap.rows;
		temp.buffer = new unsigned char[ftMainFont->glyph->bitmap.width * ftMainFont->glyph->bitmap.rows];
		copy(ftMainFont->glyph->bitmap.buffer, ftMainFont->glyph->bitmap.buffer + (ftMainFont->glyph->bitmap.width * ftMainFont->glyph->bitmap.rows), temp.buffer);

		charBitmapsTemp.push_back(temp);

		if (ftMainFont->glyph->bitmap.rows > charAtlasRows)
		{
			charAtlasRows = ftMainFont->glyph->bitmap.rows;
		}
		if (ftMainFont->glyph->bitmap.width > widthPerCharacter)
		{
			widthPerCharacter = ftMainFont->glyph->bitmap.width;
		}
	}
	charAtlasWidth = charBitmapsTemp.size() * widthPerCharacter;
	
	GLubyte* charAtlasBuffer = new GLubyte[charAtlasWidth * charAtlasRows * RGBA_STRIDE];
	//build the character texture atlas
	for (size_t i = 0; i < charAtlasWidth * charAtlasRows * RGBA_STRIDE; i++)//init it all to zero
	{
		charAtlasBuffer[i] = 0;
	}

	size_t offset = 0; //x-offset of the top-left pixel of the image that we are currently copying over to the atlas
	for (size_t i = 0; i < charBitmapsTemp.size(); i++) //add each character to the atlas
	{
		FT_Bitmap* bitmap = &charBitmapsTemp.at(i);
		size_t dataLength = (static_cast<size_t>(bitmap->width) * bitmap->rows);

		for (size_t j = 0; j < bitmap->rows; j++) //for each row
		{
			for (size_t k = 0; k < bitmap->width; k++) //for each pixel
			{
				size_t pix = offset * RGBA_STRIDE + (j * charAtlasWidth * RGBA_STRIDE) + (k * RGBA_STRIDE);
				charAtlasBuffer[pix + 0] = bitmap->buffer[j * bitmap->width + k]; //R
				charAtlasBuffer[pix + 1] = bitmap->buffer[j * bitmap->width + k]; //G
				charAtlasBuffer[pix + 2] = bitmap->buffer[j * bitmap->width + k]; //B
				charAtlasBuffer[pix + 3] = bitmap->buffer[j * bitmap->width + k]; //A
			}
		}
		offset += widthPerCharacter;
	}
	//stbi_write_bmp("Textures/TestOut.bmp", charAtlasWidth, charAtlasRows, RGBA_STRIDE, charAtlasBuffer); //for testing, manually inspecting the char atlas. TODO remove me

	//textures (from file)
	auto blankWhiteTex = Texture("textures/blankwhite.png");
	auto greeblingTex = Texture("textures/ship.png");
	auto cubeTex = Texture("textures/colored_johncat.bmp");
	
	//texture (from texture atlas)
	auto charAtlasTex = Texture(charAtlasBuffer, charAtlasWidth, charAtlasRows);

	//2d renderers
	auto newGameTextBox = TextBox(std::string("NEW GAME"), cubeTex.handle, textureLocation2D, translationLocation2D, glm::vec2(0,0), glm::vec2(.005, .005));

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

	std::map<std::string, TextBox*> renderer2ds = {
		{"newGameText",&newGameTextBox}
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