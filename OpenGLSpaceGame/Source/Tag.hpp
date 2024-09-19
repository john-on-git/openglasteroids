#pragma once

//In its own file for a reason. Included by multiple other files.
enum tag { //These are applied to game objects and determine their properties. Can't just be classes as I don't intend for them to be mutually exclusive, but may be worth handling this in an object-oriented way.
	SHIP,
	SHIP_PROJECTILE,
	ASTEROID,
	ALIEN,
	ALIEN_PROJECTILE
};