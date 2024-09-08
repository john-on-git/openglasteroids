#pragma once

#include "AppState.hpp"
#include "../Model/Model.hpp"
#include "../Program/program.hpp"

class MainMenu : public AppState
{
	protected:
		Model* asteroidModel;
		Model* projectileModel;
		Model* shipModel;

		GLuint colorLocation;
		GLuint projectionLocation;
		GLuint viewLocation;
		GLuint modelLocation;

		Program* texturedColoredShader;
		Program* blockColorShader;
	public:
		MainMenu(void (*SetState)(AppState*), bool keyPressed[360], Model* asteroidModel, Model* projectileModel, Model* shipModel, GLuint colorLocation, GLuint modelLocation, GLuint projectionLocation, GLuint viewLocation, Program* texturedColoredShader, Program* blockColorShader);
		// Inherited via AppState
		virtual void Tick() override;
};

