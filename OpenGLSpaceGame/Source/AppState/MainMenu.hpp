#pragma once

#include "AppState.hpp"
#include "../Model/Model.hpp"
#include "../Program/Program.hpp"
#include "../TextBox/TextBox.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

class MainMenu : public AppState
{
	protected:
		TextBox* newGameRenderer;
		TextBox* highScoresRenderer;


		Program* textShader2D;
	public:
		MainMenu(glm::vec2* cursorPos, bool mousePressed[8], TextBox* newGameTextBox, TextBox* highScoresTextBox, Program* textShader2D);
		// Inherited via AppState
		virtual void OnEntry() override;
		virtual SwitchState Tick() override;
};

