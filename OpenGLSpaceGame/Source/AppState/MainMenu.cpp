#include "MainMenu.hpp"

#include <GLFW/glfw3.h>

MainMenu::MainMenu(bool keyPressed[360], glm::vec2* cursorPos, bool mousePressed[8], TextBox* newGameTextBox, TextBox* highScoresTextBox, GLuint colorLocation, GLuint modelViewLocation, Program* texturedColoredShader, Program* blockColorShader, Program* textShader2D) : AppState(SetState, keyPressed, cursorPos, mousePressed)
{

	this->models = models;
	this->renderer2Ds = renderer2Ds;
	this->newGameRenderer = newGameTextBox;
	this->highScoresRenderer = highScoresTextBox;

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
SwitchState MainMenu::Tick()
{
	//new game hover/click logic
	if (newGameRenderer->InBounds(*cursorPos)) //if hovering
	{
		if (mousePressed[GLFW_MOUSE_BUTTON_1]) //and clicking
		{
			return GAME_IN_PROGRESS; //change state
		}
		else //do the onhover effect
		{
			newGameRenderer->colorMask = glm::vec4(.5, .5, .5, 1);
		}
	}
	else {
		newGameRenderer->colorMask = glm::vec4(1, 1, 1, 1); //not hovering, so set its color back to normal
	}

	//high scores hover/click logic
	if (highScoresRenderer->InBounds(*cursorPos)) //if hovering
	{
		if (mousePressed[GLFW_MOUSE_BUTTON_1]) //and clicking
		{
			return VIEW_HIGH_SCORES; //change state
		}
		else //do the onhover effect
		{
			highScoresRenderer->colorMask = glm::vec4(.5, .5, .5, 1);
		}
	}
	else {
		highScoresRenderer->colorMask = glm::vec4(1, 1, 1, 1); //not hovering, so set its color back to normal
	}

	newGameRenderer->Draw();
	highScoresRenderer->Draw();

	return UNCHANGED;
}
