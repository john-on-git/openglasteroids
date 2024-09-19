#include "GameOver.hpp"

#include <GLFW/glfw3.h>

constexpr auto SCORE_COUNTER_SIZE = glm::vec2(0.05f, 0.05f);
constexpr auto SCORE_COUNTER_POSITION = glm::vec2(0.4,0);

GameOver::GameOver(size_t score, glm::vec2* cursorPos, bool mousePressed[8], TextBox* scoreTextBox, TextBox* mainMenuTextBox, Program* textShader2D, GLuint textureAtlasHandle, GLuint textureLocation2D, GLuint translationLocation2D, GLuint colorMaskLocation2D, glm::vec2* windowDimensions) : AppState(SetState, keyPressed, cursorPos, mousePressed)
{
	this->scoreTextBox = scoreTextBox;
	scoreValueTextBox = new TextBox(std::to_string(score), textureAtlasHandle, textureLocation2D, translationLocation2D, colorMaskLocation2D, SCORE_COUNTER_POSITION, SCORE_COUNTER_SIZE, windowDimensions);
	
	this->mainMenuTextBox = mainMenuTextBox;

	this->textShader2D = textShader2D;
}

GameOver::~GameOver()
{
	delete scoreValueTextBox;
}

void GameOver::OnEntry()
{
	textShader2D->Use();
}
SwitchState GameOver::Tick()
{
	//close hover/click logic
	if (mainMenuTextBox->InBounds(*cursorPos)) //if hovering
	{
		if (mousePressed[GLFW_MOUSE_BUTTON_1]) //and clicking
		{
			return SwitchState{ MAIN_MENU,0 }; //change state
		}
		else //do the onhover effect
		{
			mainMenuTextBox->colorMask = glm::vec4(.5, .5, .5, 1);
		}
	}
	else {
		mainMenuTextBox->colorMask = glm::vec4(1, 1, 1, 1); //not hovering, so set its color back to normal
	}

	scoreTextBox->Draw();
	scoreValueTextBox->Draw();
	mainMenuTextBox->Draw();

	return SwitchState{UNCHANGED,0};
}
