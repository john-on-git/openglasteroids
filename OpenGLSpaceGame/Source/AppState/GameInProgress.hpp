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
		Model* asteroidModel;
		Model* shipProjectileModel;
		Model* shipModel;
		Model* alienModel;
		Model* alienProjectileModel;

		TextBox* scoreValueTextBox;


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

		unsigned char numAsteroids;
		unsigned char asteroidSide;

		unsigned char numAliens;
		unsigned char numTargetAliens;
		unsigned char alienSide;

		unsigned char showDebugInfoToggleDelay;
		bool showDebugInfo;

		unsigned short score;


		QuadTreeCollisionHandler* collisionHandler;

		size_t time;
	public:
		GameInProgress(bool keyPressed[360], glm::vec2* cursorPos, bool mousePressed[8], Model* asteroidModel, Model* shipProjectileModel, Model* shipModel, Model* alienModel, Model* alienProjectileModel, GLuint colorLocation, GLuint modelViewLocation, Program* texturedColoredShader, Program* blockColorShader, Program* textShader2D, GLuint textureAtlasHandle, GLuint textureLocation2D, GLuint translationLocation2D, GLuint colorMaskLocation2D, glm::vec2* windowDimensions);
		virtual void OnEntry() override;
		SwitchState Tick();
};

