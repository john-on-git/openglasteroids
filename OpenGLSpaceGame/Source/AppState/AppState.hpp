#pragma once
class AppState
{
	protected:
		void (*SetState)(AppState*);
		bool* keyPressed;
	public:
		virtual void OnEntry() = 0;
		virtual void Tick() = 0;
};

