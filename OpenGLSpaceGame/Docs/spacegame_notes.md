- TODO
	1. texture support			 ✓
	2. loading models from file ✓
	3. licensing stuff for libs ✓
	4. fix memory leak			 ✓
	5. fix bullet shooting		 ✓
	6. collision detection		
	7. text rendering?
		1. Render anything 2d on screen. DONE
			1. ISSUE: ftMainFont->glyph->bitmap.buffer is nullptr. CAUSE: was not calling FT_Render_Glyph
			2. ISSUE: crashes in Texture constructor. Probably because the bitmap is in the wrong format. CAUSE: old logic free'd the bitmap, causes a stack free bc it's no longer a pointer
			3. ISSUE: doesn't crash but nothing renders. Prob an issue with the vert shader, or rendering something really wacky bc of the bitmap format. CAUSE: 2d text shader was not being used.
		2. Position it. DONE
		3. Render one character.
			1. ISSUE: render text images all corrupt, straight out of freetype. CAUSE: data was in unexpected foramt (single-channel instead of four-channel RGBA), error was fixed by converting to RGBA before passing to glTexImage
			2. ISSUE: charatlas texture doesn't work, other textures do. CAUSE: texture was really large (200 MB). Guess it doesn't work at that size for some reason? GL issue?
			DONE.
		4. Render a string of characters.
			1. ISSUE: when making a textbox w/ multiple chars, just the first char renders. Cause: values in element array buffer were calculated wrong + draw call was set up to draw only the first 6 verts as in test
			2. ISSUE: charatlas texture now loads but is misaligned. Also it's still kind of big. CAUSE: calculating num supported chars wrong.
			DONE
		5. menu buttons on-click behaviour (and on-hover feedback, turn grey + sfx).
		6. alien ship enemies.
		7. high scores menu (optional).
		
		8. fix UV texture misalignment bug on asteroids (UV error or something systemic?)
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
18.9.24
	- It's going to be a little bit hacky towards the end. I want this finished and it won't be maintained, so I don't particularly care about finding generalisations. 
	  I suppose it must look clean if anyone's going to look at it? Hopefully they understand the call.
	- Ok so the state pattern won't work because it introduces circular dependencies between states. Should have thought about this before but at least it's been caught now.
		- This has been resolved by moving some responsibilites to the context.
	- I should refactor the game objects to improve the encapsulation because currently it's a bit of a mess.
	  For the sake of time it is probably better if I just have them inherit rather than trying to come up with a composition-based design.
	  No, after giving it some more thought I've recalled the original reason for this design choice: the objects must share so much info that encapsulating it doesn't really give us
	  much additional clarity. If they need to share an interface on Tick(), it would require a bunch of arguments, and they need to modify the state. It could be passed in, or taken as a return value?
	  Either way I don't think it's really that much clearer than an imperative style. It splits everything into a dozen different files, and the generalisations provide very little benefit since none of it will be modified.
	- WorldObject.getPosition, this is nullptr? man I give up for today
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
	