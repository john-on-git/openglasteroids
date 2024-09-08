#include "MainMenu.hpp"
#include "GameInProgress.hpp"

#include <GLFW/glfw3.h>

MainMenu::MainMenu(void (*SetState)(AppState*), bool keyPressed[360], Model* asteroidModel, Model* projectileModel, Model* shipModel, GLuint colorLocation, GLuint modelLocation, GLuint projectionLocation, GLuint viewLocation, Program* texturedColoredShader, Program* blockColorShader)
{
	this->SetState = SetState;
	this->keyPressed = keyPressed;

	this->asteroidModel = asteroidModel;
	this->projectileModel = projectileModel;
	this->shipModel = shipModel;

	this->colorLocation = colorLocation;
	this->modelLocation = modelLocation;
	this->projectionLocation = projectionLocation;
	this->viewLocation = viewLocation;

	this->texturedColoredShader = texturedColoredShader;
	this->blockColorShader = blockColorShader;
}

void MainMenu::Tick()
{
	if (keyPressed[GLFW_KEY_ENTER]) //TODO placeholder replace once clicky buttons implemented
	{
		cout << "Main Menu -> GameInProgress" << endl;
		SetState(new GameInProgress(SetState, keyPressed, asteroidModel, projectileModel, shipModel, colorLocation, modelLocation, projectionLocation, viewLocation, texturedColoredShader, blockColorShader));
	}
}
