#pragma once
#include "glm/vec2.hpp"

enum SwitchState
{
	UNCHANGED = 0,
	MAIN_MENU,
	GAME_IN_PROGRESS,
	VIEW_HIGH_SCORES
};

class AppState
{
	protected:
		void (*SetState)(AppState*);
		bool* keyPressed;
		glm::vec2* cursorPos;
		bool* mousePressed;
	public:
		AppState(void (*SetState)(AppState*), bool keyPressed[360], glm::vec2* cursorPos, bool mousePressed[8]);
		virtual void OnEntry() = 0;
		virtual SwitchState Tick() = 0;
};

