#pragma once
#include "glm/vec2.hpp"

enum MENU_STATE
{
	UNCHANGED = 0,
	MAIN_MENU,
	GAME_IN_PROGRESS,
	GAME_OVER,
	VIEW_HIGH_SCORES
};

struct SwitchState
{
	MENU_STATE nextState;
	size_t score;
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
		virtual ~AppState();
		virtual void OnEntry() = 0;
		virtual SwitchState Tick() = 0;
};

