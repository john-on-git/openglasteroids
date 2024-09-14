#include "MainMenu.hpp"
#include "GameInProgress.hpp"
#include "../TextBox/TextBox.hpp"

#include <GLFW/glfw3.h>

MainMenu::MainMenu(void (*SetState)(AppState*), bool keyPressed[360], std::map<std::string,Model*>* models, std::map<std::string, TextBox*>* renderer2Ds, GLuint colorLocation, GLuint modelViewLocation, Program* texturedColoredShader, Program* blockColorShader, Program* textShader2D)
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
	this->textShader2D = textShader2D;

	this->ftLibrary = ftLibrary;
	this->ftMainFont = ftMainFont;
}

void MainMenu::OnEntry()
{
	textShader2D->Use();
}
void MainMenu::Tick()
{
	newGameRenderer->Draw();
	if (keyPressed[GLFW_KEY_ENTER]) //TODO placeholder replace once clicky buttons implemented
	{
		SetState(new GameInProgress(SetState, keyPressed, models, renderer2Ds, colorLocation, modelViewLocation, texturedColoredShader, blockColorShader, textShader2D));
	}
}
