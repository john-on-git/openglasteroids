#include "AppState.hpp"

AppState::AppState(void (*SetState)(AppState*), bool keyPressed[360], glm::vec2* cursorPos, bool mousePressed[8])
{
	this->SetState = SetState;
	this->keyPressed = keyPressed;
	this->cursorPos = cursorPos;
	this->mousePressed = mousePressed;
}