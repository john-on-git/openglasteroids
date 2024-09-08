#pragma once

#include "AppState.hpp"
#include "../WorldObject/SpaceGameObject.hpp"
#include "../Program/Program.hpp"
#include "../QuadTreeCollisionHandler/QuadTreeCollisionHandler.hpp"

class GameInProgress : public AppState
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

		SpaceGameObject* ship;
		vector<SpaceGameObject*> objects;


		unsigned char numAsteroids;
		unsigned char asteroidSide;
		unsigned char fireDelay;
		unsigned char controlsDisabled;
		unsigned char showDebugInfoToggleDelay;
		bool showDebugInfo;

		QuadTreeCollisionHandler* collisionHandler;

		static constexpr auto COLOR_FLASH = glm::vec4(2.0f, 2.0f, 0.25f, 1.0f);
		static constexpr auto COLOR_NONE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	public:
		GameInProgress(void (*SetState)(AppState*), bool keyPressed[360], Model* asteroidModel, Model* projectileModel, Model* shipModel, GLuint colorLocation, GLuint modelLocation, GLuint projectionLocation, GLuint viewLocation, Program* texturedColoredShader, Program* blockColorShader);
		void Tick();
};

