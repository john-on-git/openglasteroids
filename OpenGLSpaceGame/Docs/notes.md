TODO
	texture support			 ✓
	loading models from file ✓
	licensing stuff for libs ✓
	fix memory leak			 ✓
	fix bullet shooting		 ✓
	collision detection		
15.5.2024
	- Came back to the project after several years to restart work.
	- Fixed an issue with the input handling.
	- Starting work on the collision, looks real hard. I think it should be easy as long as I apply a proper testing strategy.
18.7.2024
	- what was I doing I forgor 💀.
	- init the face data in Model::Model (need to calculate d).													✓
	- using the face data, write the collision logic in QuadTreeCollisionHandler::LineIntersectsPolygon
	- first attempt at implementation, doesn't work

	- Delta rework, "a pointer to a bound function may only be used to call the function", needs fixed, nontrivial.
26.7.2024
	- Got the deltas working, started work on the collision detection but it's still broke.
	- Needs to be unit-tested but oooohhh my gawddd, do not want to. IDK no way I'm figuring it out without that tho.
	- *think* it's an issue with the plane equations, but it's quite simple so idk how it could be...
1.8.2024
	- The linking issues are something to do with the project.
	- TODO: make a fresh project and install everything with vcpkg

What's the best way to implement projectile decay?
	- Property of WorldObject? Nah, should inherit/compose: not really a property of all worldobjects.
	- Child class of WorldObject?
	- Decay object.
		- Composition w/ WorldObject?
		- Just used in main
	- imperative in main.cpp

- Delta cleanup
	- DeltaAttachable class, go through tree. When an provider/ referencing the target is located, 


- At points I return vector<T*>*, then delete it after use. It should be fine to return a vector<T*>, passing by value.

- SpaceGameObject : WorldObject, has additional properties (vector<Delta> inComingDeltas).
	- Make deltas only have one target.
	- Move all deltas onto these collections.
	- When an object is destroyed, the inComingDeltas as destroyed too.
	- Should really just use inheritance to get the funcitonality of different object types, composition system rn is rubbish.

- Freetype for text rendering?
	
- Actual Requirements.
	- Constant 144fps on my machine.
	- Main Menu.
		- Start Game.
		- High Scores.
	- High Score Screen.
		- Persistent.
		- Name (4 chars), Score (int 0-9999).
	- Gameplay.
		- Score
		- Asteroids.
			- Provide terrain. Stun on contact.
		- Alien Ship.
			- Green & dounded, teardrop-shaped.
			- Tries to keep distance from the hero ship. Shoots at it, with random spray.
			- 100 points for destroying.
		- Power Ups.
			- Rapid Fire
			- Homing Shots.
			- Invincibility.
		- Game Ends When 0 lives or 9999 score.
		- AFter game ends, user is prompted to enter name for score.
	