#pragma once

#include "AppState.hpp"
#include "../WorldObject/SpaceGameObject.hpp"
#include "../Program/Program.hpp"
#include "../TextBox/TextBox.hpp"
#include "../QuadTreeCollisionHandler/QuadTreeCollisionHandler.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>


class GameInProgress : public AppState
{
	protected:
		Model** asteroidModels;
		Model* projectileModel;
		Model* shipModel;
		Model* alienModel;

		TextBox* scoreValueTextBox;


		GLuint textureAtlasHandle;
		GLuint textureLocation2D; 
		GLuint translationLocation2D; 
		GLuint colorMaskLocation2D;
		GLuint modelViewLocation;
		glm::vec2* windowDimensions;
		

		Program* texturedColoredShader;
		Program* blockColorShader;
		Program* textShader2D;

		SpaceGameObject* ship;
		vector<SpaceGameObject*> objects;

		unsigned char numAsteroids;
		unsigned char asteroidSide;

		unsigned char numAliens;
		unsigned char numTargetAliens;
		unsigned char alienSide;

		unsigned short score;

		QuadTreeCollisionHandler* collisionHandler;
	public:
		GameInProgress(bool keyPressed[360], glm::vec2* cursorPos, bool mousePressed[8], Model** asteroidModels, Model* projectileModel, Model* shipModel, Model* alienModel, GLuint modelViewLocation, Program* texturedColoredShader, Program* textShader2D, GLuint textureAtlasHandle, GLuint textureLocation2D, GLuint translationLocation2D, GLuint colorMaskLocation2D, glm::vec2* windowDimensions);
		~GameInProgress();
		virtual void OnEntry() override;
		virtual void Draw() override;
		virtual SwitchState Tick(size_t time) override;
};

