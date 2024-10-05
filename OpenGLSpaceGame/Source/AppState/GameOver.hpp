#pragma once

#include "AppState.hpp"
#include "../Model/Model.hpp"
#include "../Program/Program.hpp"
#include "../TextBox/TextBox.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

class GameOver : public AppState
{
	protected:
		TextBox* scoreTextBox;
		TextBox* scoreValueTextBox;
		TextBox* mainMenuTextBox;

		Program* textShader2D;
	public:
		GameOver(size_t score, glm::vec2* cursorPos, bool mousePressed[8], TextBox* scoreTextBox, TextBox* mainMenuTextBox, Program* textShader2D, GLuint textureAtlasHandle, GLuint textureLocation2D, GLuint translationLocation2D, GLuint colorMaskLocation2D, glm::vec2* windowDimensions);
		~GameOver();
		// Inherited via AppState
		virtual void OnEntry() override;
		virtual void Draw() override;
		virtual SwitchState Tick(size_t time) override;
};

