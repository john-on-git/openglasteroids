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
		Model* projectileModel;
		Model* shipModel;

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
		unsigned char fireDelay;
		unsigned char controlsDisabled;
		unsigned char showDebugInfoToggleDelay;
		bool showDebugInfo;


		QuadTreeCollisionHandler* collisionHandler;

		static constexpr auto COLOR_FLASH = glm::vec4(2.0f, 2.0f, 0.25f, 1.0f);
		static constexpr auto COLOR_NONE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	public:
		GameInProgress(void (*SetState)(AppState*), bool keyPressed[360], std::map<std::string, Model*>* models, std::map<std::string, TextBox*>* renderer2Ds, GLuint colorLocation, GLuint modelViewLocation, Program* texturedColoredShader, Program* blockColorShader, Program* textShader2D);
		virtual void OnEntry() override;
		void Tick();
};

