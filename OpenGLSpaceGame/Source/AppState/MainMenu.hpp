#pragma once

#include "AppState.hpp"
#include "../Model/Model.hpp"
#include "../Program/Program.hpp"
#include "../TextBox/TextBox.hpp"
#include <ft2build.h>
#include <map>
#include FT_FREETYPE_H

class MainMenu : public AppState
{
	protected:
		std::map<std::string,Model*>* models;
		std::map<std::string, TextBox*>* renderer2Ds;
		TextBox* newGameRenderer;
		TextBox* highScoresRenderer;

		GLuint colorLocation;
		GLuint modelViewLocation;

		Program* texturedColoredShader;
		Program* blockColorShader;
		Program* textShader2D;

		FT_Library* ftLibrary;
		FT_Face* ftMainFont;
	public:
		MainMenu(bool keyPressed[360], glm::vec2* cursorPos, bool mousePressed[8], TextBox* newGameTextBox, TextBox* highScoresTextBox, GLuint colorLocation, GLuint modelViewLocation, Program* texturedColoredShader, Program* blockColorShader, Program* textShader2D);
		// Inherited via AppState
		virtual void OnEntry() override;
		virtual SwitchState Tick() override;
};

