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