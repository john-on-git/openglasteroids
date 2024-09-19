#pragma once

#include "AppState.hpp"
#include "../WorldObject/SpaceGameObject.hpp"
#include "../Program/Program.hpp"
#include "../TextBox/TextBox.hpp"
#include "../QuadTreeCollisionHandler/QuadTreeCollisionHandler.hpp"
#include <ft2build.h>
#include <map>

#include FT_FREETYPE_H

class GameInProgress : public AppState
{
	protected:
		std::map<std::string,Model*>* models;
		std::map<std::string, TextBox*>* renderer2Ds;
		Model* asteroidModel;
		Model* shipProjectileModel;
		Model* shipModel;
		Model* alienModel;
		Model* alienProjectileModel;

		TextBox* scoreCounter;


		GLuint textureAtlasHandle;
		
		GLuint textureLocation2D; 
		GLuint translationLocation2D; 
		GLuint colorMaskLocation2D; 
		glm::vec2* windowDimensions;
		
		GLuint colorLocation;
		GLuint modelViewLocation;

		Program* texturedColoredShader;
		Program* blockColorShader;
		Program* textShader2D;

		SpaceGameObject* ship;
		vector<SpaceGameObject*> objects;

		FT_Library* ftLibrary;
		FT_Face* ftMainFont;

		unsigned char numAsteroids;
		unsigned char asteroidSide;

		unsigned char numAliens;
		unsigned char numTargetAliens;
		unsigned char alienSide;

		unsigned char showDebugInfoToggleDelay;
		bool showDebugInfo;

		unsigned short score;

		QuadTreeCollisionHandler* collisionHandler;
	public:
		GameInProgress(bool keyPressed[360], glm::vec2* cursorPos, bool mousePressed[8], Model* asteroidModel, Model* shipProjectileModel, Model* shipModel, Model* alienModel, Model* alienProjectileModel, GLuint colorLocation, GLuint modelViewLocation, Program* texturedColoredShader, Program* blockColorShader, Program* textShader2D, GLuint textureAtlasHandle, GLuint textureLocation2D, GLuint translationLocation2D, GLuint colorMaskLocation2D, glm::vec2* windowDimensions);
		virtual void OnEntry() override;
		SwitchState Tick();
};

