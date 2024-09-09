#include "MainMenu.hpp"
#include "GameInProgress.hpp"

#include <GLFW/glfw3.h>

MainMenu::MainMenu(void (*SetState)(AppState*), bool keyPressed[360], std::map<std::string,Model*>* models, std::map<std::string, Renderer2D*>* renderer2Ds, GLuint colorLocation, GLuint modelViewLocation, Program* texturedColoredShader, Program* blockColorShader)
{
	this->SetState = SetState;
	this->keyPressed = keyPressed;

	this->models = models;
	this->renderer2Ds = renderer2Ds;
	this->newGameRenderer = renderer2Ds->at("newGameText");

	this->colorLocation = colorLocation;
	this->modelViewLocation = modelViewLocation;

	this->texturedColoredShader = texturedColoredShader;
	this->blockColorShader = blockColorShader;

	this->ftLibrary = ftLibrary;
	this->ftMainFont = ftMainFont;
}

void MainMenu::Tick()
{
	newGameRenderer->Draw(glm::vec2(0, 0), glm::vec2(1, 1));
	if (keyPressed[GLFW_KEY_ENTER]) //TODO placeholder replace once clicky buttons implemented
	{
		SetState(new GameInProgress(SetState, keyPressed, models, renderer2Ds, colorLocation, modelViewLocation, texturedColoredShader, blockColorShader));
	}
}
