#pragma once
class AppState
{
	protected:
		void (*SetState)(AppState*);
		bool* keyPressed;
	public:
		virtual void Tick() = 0;
};

