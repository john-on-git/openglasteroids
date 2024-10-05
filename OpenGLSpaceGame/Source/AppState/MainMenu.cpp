#include "MainMenu.hpp"

#include <GLFW/glfw3.h>

MainMenu::MainMenu(glm::vec2* cursorPos, bool mousePressed[8], TextBox* newGameTextBox, TextBox* highScoresTextBox, Program* textShader2D) : AppState(SetState, keyPressed, cursorPos, mousePressed)
{
	this->newGameRenderer = newGameTextBox;
	this->highScoresRenderer = highScoresTextBox;

	this->textShader2D = textShader2D;
}

void MainMenu::OnEntry()
{
	textShader2D->Use();
}

void MainMenu::Draw()
{
	newGameRenderer->Draw();
}

SwitchState MainMenu::Tick(size_t time)
{
	//new game hover/click logic
	if (newGameRenderer->InBounds(*cursorPos)) //if hovering
	{
		if (mousePressed[GLFW_MOUSE_BUTTON_1]) //and clicking
		{
			return SwitchState{ GAME_IN_PROGRESS,0 }; //change state
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
	//if (highScoresRenderer->InBounds(*cursorPos)) //if hovering
	//{
	//	if (mousePressed[GLFW_MOUSE_BUTTON_1]) //and clicking
	//	{
	//		return SwitchState{ VIEW_HIGH_SCORES,0 }; //change state
	//	}
	//	else //do the onhover effect
	//	{
	//		highScoresRenderer->colorMask = glm::vec4(.5, .5, .5, 1);
	//	}
	//}
	//else {
	//	highScoresRenderer->colorMask = glm::vec4(1, 1, 1, 1); //not hovering, so set its color back to normal
	//}
	//highScoresRenderer->Draw();

	return SwitchState{UNCHANGED,0};
}
