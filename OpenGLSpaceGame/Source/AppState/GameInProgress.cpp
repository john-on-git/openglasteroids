#define _USE_MATH_DEFINES
#include "GameInProgress.hpp"
#include "AppState.hpp"
#include "../main.hpp"
#include "../Delta/Delta.hpp"
#include "../Delta/DeltaProvider.hpp"
#include "../Delta/DeltaTarget.hpp"
#include "../Delta/DeltaProviders/Vec3Provider.hpp"
#include "../Delta/DeltaProviders/DragProvider.hpp"
#include "../Delta/DeltaProviders/RotDragProvider.hpp"
#include "../Delta/DeltaProviders/SpacegameObjectVelocityProvider.hpp"
#include "../Delta/DeltaProviders/SpacegameObjectRotationalVelocityProvider.hpp"
#include "../Delta/DeltaTargets/Vec3Target.hpp"
#include "../Delta/DeltaTargets/WorldObjectAngleTarget.hpp"
#include "../Delta/DeltaTargets/WorldObjectPositionTarget.hpp"
#include "../Delta/DeltaTargets/SpacegameObjectVelocityTarget.hpp"
#include "../Delta/DeltaTargets/SpacegameObjectRotationalVelocityTarget.hpp"
#include "../QuadTreeCollisionHandler/QuadTreeCollisionHandler.hpp"
#include "../WorldObject/SpaceGameObject/TemporarySpaceGameObject.hpp"
#include "../Program/program.hpp"

#include <glm/trigonometric.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

//Debug logic for visualizing the quadtree. Not very efficient but doesn't really matter
static void DrawQuadTree(bool drawAllRegions, bool drawShipRegion, bool drawShipOABB, WorldObject* ship, QuadTreeCollisionHandler* collisionHandler, Program* texturedColoredShader, Program* blockColorShader, GLuint colorLocation) {
	blockColorShader->Use();
	int STRIDE = 2;
	size_t COORDS_LEN = 8;
	size_t INDICES_LEN = 8;
	glad_glUniform4fv(colorLocation, 1, glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	unsigned int vertIndices[]{ 0, 1, 1, 2, 2, 3, 3, 0 };
	if (drawAllRegions) {
		glad_glLineWidth(1);
		vector<glm::vec2*> flattened;
		collisionHandler->GetAllBounds(&flattened);
		while (!flattened.empty()) {
			glm::vec2* bounds = flattened.back();
			//set up line quadtree debugger
			float verts[] = {
				bounds[0].x, bounds[1].y, //topleft
				bounds[1].x, bounds[1].y, //topright
				bounds[1].x, bounds[0].y, //bottomright
				bounds[0].x, bounds[0].y, //bottomleft
			};
			//generate vertex array object
			GLuint quadtreeVAO;
			glad_glGenVertexArrays(1, &quadtreeVAO);
			glad_glBindVertexArray(quadtreeVAO);
			//generate buffers and copy over data
			GLuint buffers[2]; //vertex buffer, and vertex index buffer
			glad_glGenBuffers(2, buffers); //19.5.21, first argument is the number of buffers, not the size of the buffer. corrupted the heap?

			glad_glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
			glad_glBufferData( //add verts to buffer
				GL_ARRAY_BUFFER,
				COORDS_LEN * sizeof(float),
				verts,
				GL_STATIC_DRAW
			);

			glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
			glad_glBufferData( //add verts to buffer
				GL_ELEMENT_ARRAY_BUFFER,
				INDICES_LEN * sizeof(unsigned int),
				vertIndices,
				GL_STATIC_DRAW
			);

			//set vertex attrib pointers OK
			glad_glEnableVertexAttribArray(0);
			glad_glVertexAttribPointer( //position
				0,
				2,
				GL_FLOAT,
				GL_FALSE,
				STRIDE * sizeof(float),
				nullptr
			);

			glad_glBindVertexArray(quadtreeVAO);
			glad_glDrawElements(
				GL_LINES,
				INDICES_LEN,
				GL_UNSIGNED_INT,
				nullptr
			);
			glad_glDeleteBuffers(2, buffers);
			glad_glDeleteVertexArrays(1, &quadtreeVAO);
			flattened.pop_back();
		}
	}
	if (drawShipRegion) {
		glad_glLineWidth(10);
		//set up line quadtree debugger
		auto bounds = collisionHandler->GetNodeBoundsForObject(ship);
		float verts[] = {
			bounds[0].x, bounds[1].y, //topleft
			bounds[1].x, bounds[1].y, //topright
			bounds[1].x, bounds[0].y, //bottomright
			bounds[0].x, bounds[0].y, //bottomleft
		};
		//generate vertex array object
		GLuint quadtreeVAO;
		glad_glGenVertexArrays(1, &quadtreeVAO);
		glad_glBindVertexArray(quadtreeVAO);
		//generate buffers and copy over data
		GLuint buffers[2]; //vertex buffer, and vertex index buffer
		glad_glGenBuffers(2, buffers); //19.5.21, first argument is the number of buffers, not the size of the buffer. corrupted the heap?

		glad_glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glad_glBufferData( //add verts to buffer
			GL_ARRAY_BUFFER,
			COORDS_LEN * sizeof(float),
			verts,
			GL_STATIC_DRAW
		);

		glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glad_glBufferData( //add verts to buffer
			GL_ELEMENT_ARRAY_BUFFER,
			INDICES_LEN * sizeof(unsigned int),
			vertIndices,
			GL_STATIC_DRAW
		);


		//set vertex attrib pointers OK
		glad_glEnableVertexAttribArray(0);
		glad_glVertexAttribPointer( //position
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			STRIDE * sizeof(float),
			nullptr
		);

		glad_glBindVertexArray(quadtreeVAO);
		glad_glDrawElements(
			GL_LINES,
			INDICES_LEN,
			GL_UNSIGNED_INT,
			nullptr
		);
		glad_glDeleteBuffers(2, buffers);
		glad_glDeleteVertexArrays(1, &quadtreeVAO);
	}

	if (drawShipOABB)
	{
		glad_glUniform4fv(colorLocation, 1, glm::value_ptr(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));
		glad_glLineWidth(5);
		//set up line quadtree debugger
		glm::vec3* bounds = ship->getOrientedBoundingBox();
		float verts[] = {
			bounds[0].x, bounds[0].y, //topleft
			bounds[1].x, bounds[1].y, //topright
			bounds[7].x, bounds[7].y, //bottomright
			bounds[6].x, bounds[6].y, //bottomleft
		};
		//generate vertex array object
		GLuint quadtreeVAO;
		glad_glGenVertexArrays(1, &quadtreeVAO);
		glad_glBindVertexArray(quadtreeVAO);
		//generate buffers and copy over data
		GLuint buffers[2]; //vertex buffer, and vertex index buffer
		glad_glGenBuffers(2, buffers); //19.5.21, first argument is the number of buffers, not the size of the buffer. corrupted the heap?

		glad_glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glad_glBufferData( //add verts to buffer
			GL_ARRAY_BUFFER,
			COORDS_LEN * sizeof(float),
			verts,
			GL_STATIC_DRAW
		);

		glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glad_glBufferData( //add verts to buffer
			GL_ELEMENT_ARRAY_BUFFER,
			INDICES_LEN * sizeof(unsigned int),
			vertIndices,
			GL_STATIC_DRAW
		);

		//set vertex attrib pointers OK
		glad_glEnableVertexAttribArray(0);
		glad_glVertexAttribPointer( //position
			0,
			2,
			GL_FLOAT,
			GL_FALSE,
			STRIDE * sizeof(float),
			nullptr
		);

		glad_glBindVertexArray(quadtreeVAO);
		glad_glDrawElements(
			GL_LINES,
			INDICES_LEN,
			GL_UNSIGNED_INT,
			nullptr
		);
		glad_glDeleteBuffers(2, buffers);
		glad_glDeleteVertexArrays(1, &quadtreeVAO);
	}
	texturedColoredShader->Use();
}

constexpr auto SCORE_COUNTER_SIZE = glm::vec2(0.05f, 0.05f);
constexpr auto SCORE_COUNTER_POSITION = glm::vec2(-0.9f, -0.9f);

GameInProgress::GameInProgress(bool keyPressed[360], glm::vec2* cursorPos, bool mousePressed[8], Model* asteroidModel, Model* shipProjectileModel, Model* shipModel, Model* alienModel, Model* alienProjectileModel, GLuint colorLocation, GLuint modelViewLocation, Program* texturedColoredShader, Program* blockColorShader, Program* textShader2D, GLuint textureAtlasHandle, GLuint textureLocation2D, GLuint translationLocation2D, GLuint colorMaskLocation2D, glm::vec2* windowDimensions) : AppState(SetState, keyPressed, cursorPos, mousePressed)
{
	this->asteroidModel = asteroidModel;
	this->shipProjectileModel = shipProjectileModel;
	this->alienProjectileModel = alienProjectileModel;
	this->shipModel = shipModel;
	this->alienModel = alienModel;

	this->scoreValueTextBox = new TextBox("000", textureAtlasHandle, textureLocation2D, translationLocation2D, colorMaskLocation2D, SCORE_COUNTER_POSITION, SCORE_COUNTER_SIZE, windowDimensions);

	this->textureAtlasHandle = textureAtlasHandle;

	this->textureLocation2D = textureLocation2D;
	this->translationLocation2D = translationLocation2D;
	this->colorMaskLocation2D = colorMaskLocation2D;

	this->windowDimensions = windowDimensions;

	this->colorLocation = colorLocation;
	this->modelViewLocation = modelViewLocation;

	this->texturedColoredShader = texturedColoredShader;
	this->blockColorShader = blockColorShader;
	this->textShader2D = textShader2D;

	//set up game & world stuff
	ship = new SpaceGameObject(
		shipModel,
		glm::vec3(0.0f, 0.0f, 0.0f),	//pos
		glm::vec3(0.0f, 0.0f, 0.0f),	//vel
		glm::vec3(0.0f, 0.0f, 0.0f),	//rot vel
		glm::vec3(90.0f, 0.0f, 0.0f),	//rot
		glm::vec3(0.05f, 0.05f, 0.05f), //scale
		modelViewLocation,
		unordered_set<tag>{ SHIP }
	);
	//the ship has drag
	ship->velocityDelta->addDependent(new Delta<glm::vec3>(new DragProvider(ship), { new SpaceGameObjectVelocityTarget(ship) }, {}));
	ship->rotationalVelocityDelta->addDependent(new Delta<glm::vec3>(new RotDragProvider(ship, 10), { new SpaceGameObjectRotationalVelocityTarget(ship)}, {}));

	objects.push_back(ship);

	numAsteroids = 0;
	asteroidSide = 0;

	numAliens = 0;
	numTargetAliens = INITIAL_NUM_TARGET_ALIENS;
	alienSide = 0;

	score = 0;

	collisionHandler = new QuadTreeCollisionHandler(
		10,
		glm::vec2(-10.1f, -10.1f),
		glm::vec2(10.1f, 10.1f)
	);
	for (auto object : objects) //register all objects
	{
		collisionHandler->Register(object);
	}

	auto shipAngleTarget = WorldObjectAngleTarget(ship);

	std::unordered_set<WorldObject*> stunned = {};

	showDebugInfo = false;
	showDebugInfoToggleDelay = 0;

	time = 1;
}
void GameInProgress::OnEntry()
{
	texturedColoredShader->Use();
}
SwitchState GameInProgress::Tick()
{
	time++;
	unsigned short newScore = score; //for tracking changes in score this tick, to determine whether we need to update the score counter

	glm::vec2 arenaDimensions = glm::vec2(
		(static_cast<float>(windowDimensions->x) / min(windowDimensions->x, windowDimensions->y)) * 1.1f,
		(static_cast<float>(windowDimensions->y) / min(windowDimensions->x, windowDimensions->y)) * 1.1f
	);

	while (numAsteroids < NUM_TARGET_ASTEROIDS)
	{
		//asteroids launch from a different side each time rotating counterclockwise, from a random point on the side, towards the center, +-45°
		float rel = ((float)rand()) / RAND_MAX;
		float angle = 0;
		float scale = 0.03 + ((float)rand()) / RAND_MAX * 0.1;
		glm::vec3 pos;
		switch (asteroidSide)
		{
		case 0:
			pos = glm::vec3(-.95f * arenaDimensions.x, rel, 0.0f); //left
			angle = 0 - (M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //right
			break;
		case 1:
			pos = glm::vec3(rel, -.95f * arenaDimensions.y, 0.0f); //bottom
			angle = (M_PI / 2) - (M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //up
			break;
		case 2:
			pos = glm::vec3(.95f * arenaDimensions.x, rel, 0.0f); //right
			angle = (M_PI)-(M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //left
			break;
		case 3:
			pos = glm::vec3(rel, .95f * arenaDimensions.y, 0.0f); //top
			angle = (M_PI * 3 / 2) - (M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //down
			break;
		}
		SpaceGameObject* asteroid = new SpaceGameObject(
			asteroidModel,
			pos,	//pos
			glm::vec3(ASTEROID_INIT_VEL * cos(angle), ASTEROID_INIT_VEL * sin(angle), 0.0f),	//vel
			glm::vec3(((float)rand()) / RAND_MAX * .5, ((float)rand()) / RAND_MAX * .5, ((float)rand()) / RAND_MAX * .5),	//rot vel
			glm::vec3(0.0f, 0.0f, 0.0f),	//rot
			glm::vec3(scale, scale, scale),	//scale
			modelViewLocation,
			unordered_set<tag>{ ASTEROID }
		);
		objects.push_back(asteroid);
		collisionHandler->Register(asteroid);
		numAsteroids++;
		++asteroidSide %= 4;
	}

	//add another alien every so often to increase the difficulty
	if (time % ALIEN_CAP_INCREASE_PERIOD == 0 && numTargetAliens<MAX_ALIENS)
	{
		numTargetAliens++;
	}

	//replace any defeated aliens
	while (numAliens < numTargetAliens)
	{
		//asteroids launch from a different side each time rotating counterclockwise, from a random point on the side, towards the center, +-45°
		float rel = ((float)rand()) / RAND_MAX;
		float angle = 0;
		glm::vec3 pos;
		switch (alienSide)
		{
			case 0:
				pos = glm::vec3(-.95f * arenaDimensions.x, rel, 0.0f); //left
				angle = 0 - (M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //right
				break;
			case 1:
				pos = glm::vec3(rel, -.95f * arenaDimensions.y, 0.0f); //bottom
				angle = (M_PI / 2) - (M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //up
				break;
			case 2:
				pos = glm::vec3(.95f * arenaDimensions.x, rel, 0.0f); //right
				angle = (M_PI)-(M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //left
				break;
			case 3:
				pos = glm::vec3(rel, .95f * arenaDimensions.y, 0.0f); //top
				angle = (M_PI * 3 / 2) - (M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //down
				break;
		}
		SpaceGameObject* alien = new SpaceGameObject(
			alienModel,
			pos,	//pos
			glm::vec3(0.0f, 0.0f, 0.0f),	//vel
			glm::vec3(0.0f, 0.0f, 0.0f),	//rot vel
			glm::vec3(90.0f, 0.0f, 0.0f),	//rot
			glm::vec3(0.05f,0.05f,0.05f),	//scale
			modelViewLocation,
			std::unordered_set<tag> { ALIEN }
		);
		alien->fireDelay = ALIEN_GRACE_PERIOD - FPS + (FPS * ((float)rand()) / RAND_MAX);
		objects.push_back(alien);
		collisionHandler->Register(alien);
		numAliens++;
		++alienSide %= 4;
	}
	if (!ship->IsStunned())
	{
		//get player input
		auto rad = glm::radians(ship->getAngle().y-90);

		if (ship != nullptr && (keyPressed[GLFW_KEY_W] || keyPressed[GLFW_KEY_UP]))
		{
			ship->velocityDelta->addDependent(new Delta<glm::vec3>(new Vec3Provider(glm::vec3(SHIP_ACCELERATION * cos(rad), SHIP_ACCELERATION* sin(rad), 0.0f)), {new SpaceGameObjectVelocityTarget(ship)}, {}, 10, SHIP_MAX_VELOCITY));
		}
		else if (ship != nullptr && (keyPressed[GLFW_KEY_S] || keyPressed[GLFW_KEY_DOWN]))
		{
			ship->velocityDelta->addDependent(new Delta<glm::vec3>(new Vec3Provider(glm::vec3(-SHIP_ACCELERATION * cos(rad), -SHIP_ACCELERATION * sin(rad), 0.0f)), { new SpaceGameObjectVelocityTarget(ship) }, {}, 10, SHIP_MAX_VELOCITY));
		}

		if (ship != nullptr && (keyPressed[GLFW_KEY_A] || keyPressed[GLFW_KEY_LEFT]))
		{
			ship->rotationalVelocityDelta->addDependent(new Delta<glm::vec3>(
				new Vec3Provider(glm::vec3(0.0f, -SHIP_TURNRATE_MULT, 0.0f)),
				{ new SpaceGameObjectRotationalVelocityTarget(ship) },
				{},
				1
			));
		}
		else if (ship != nullptr && (keyPressed[GLFW_KEY_D] || keyPressed[GLFW_KEY_RIGHT]))
		{
			ship->rotationalVelocityDelta->addDependent(new Delta<glm::vec3>(
				new Vec3Provider(glm::vec3(0.0f, SHIP_TURNRATE_MULT, 0.0f)),
				{ new SpaceGameObjectRotationalVelocityTarget(ship)},
				{},
				1
			));
		}

		if (ship != nullptr && (keyPressed[GLFW_KEY_Z] || keyPressed[GLFW_KEY_SPACE]) && ship->fireDelay == 0)
		{
			glm::vec3 shipVelocity = ship->getVelocity();
			TemporarySpaceGameObject* shipProjectile = new TemporarySpaceGameObject(
				shipProjectileModel,
				glm::vec3(
					ship->getPosition().x + (cos(rad) * 0.07), //second half moves the spawn point away from the center of the ship
					ship->getPosition().y + (sin(rad) * 0.07), //0.05 is the distance between the center and tip
					ship->getPosition().z
				),
				glm::vec3(
					shipVelocity.x + (cos(rad) * PROJECTILE_VELOCITY_MULT),
					shipVelocity.y + (sin(rad) * PROJECTILE_VELOCITY_MULT),
					shipVelocity.z + 0.0f
				),
				glm::vec3(0, 0, 0),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.005f, 0.005f, 0.005f), //glm::vec3(0.005f, 0.005f, 0.005f),
				modelViewLocation,
				unordered_set<tag>{ SHIP_PROJECTILE },
				PROJECTILE_DURATION
			);
			objects.push_back(shipProjectile);
			collisionHandler->Register(shipProjectile);

			ship->fireDelay = SHIP_FIRE_DELAY; //reset fire delay
		}
	}

	//toggle debug info
	if (showDebugInfoToggleDelay > 0) { showDebugInfoToggleDelay--; }
	if (keyPressed[GLFW_KEY_F5] && showDebugInfoToggleDelay == 0)
	{
		showDebugInfo = !showDebugInfo;
		showDebugInfoToggleDelay = SHOW_DEBUG_INFO_TOGGLE_DELAY;
	}


	vector<SpaceGameObject*> toAdd = vector<SpaceGameObject*>();
	for (auto object : objects)
	{
		//toroidal space
		auto objectPosition = object->getPosition();
		if (objectPosition.x > arenaDimensions.x) //off right
		{
			objectPosition.x = -arenaDimensions.x;
		}
		else if (objectPosition.x < -arenaDimensions.x) //off left
		{
			objectPosition.x = arenaDimensions.x;
		}

		if (objectPosition.y < -arenaDimensions.y) //off top
		{
			objectPosition.y = arenaDimensions.y;
		}
		else if (objectPosition.y > arenaDimensions.y) //off bottom
		{
			objectPosition.y = -arenaDimensions.y;
		}
		object->setPosition(objectPosition);

		//CPU control
		if (object->tags.count(ALIEN)==1)
		{
			if (!object->IsStunned())
			{
				glm::vec3 towardsShip = ship->getPosition() - object->getPosition(); //calculate for later

				//point towards ship
				//get the signed angle between towardsShip & the positive y-axis, this is the direction the alien should point in
				auto theta = atan2(towardsShip.y, towardsShip.x) - (M_PI / 2) + M_PI; // M_PI/2 = atan2(1,0)
				auto angleCurrent = object->getAngle().y;
				auto angleTarget = glm::degrees(theta);
				float dif = angleTarget - angleCurrent;
				//turn the other way if it's a shorter path
				if (dif > 0 && (360 - dif < dif))
				{
					dif = dif - 360;
				}
				else if (dif < 0 && (-360 - dif > dif))
				{
					dif = dif + 360;
				}

				if (dif < -HALF_ALIEN_TRACKING_CONE_DEGREES)
				{
					object->rotationalVelocityDelta->addDependent(new Delta<glm::vec3>(
						new Vec3Provider(glm::vec3(0, -ALIEN_TURNRATE_MULT, 0)),
						{ new SpaceGameObjectRotationalVelocityTarget(object) },
						{},
						1
					)); //add delta
				}
				else if (dif > HALF_ALIEN_TRACKING_CONE_DEGREES)
				{
					object->rotationalVelocityDelta->addDependent(new Delta<glm::vec3>(
						new Vec3Provider(glm::vec3(0, ALIEN_TURNRATE_MULT, 0)),
						{ new SpaceGameObjectRotationalVelocityTarget(object) },
						{},
						1
					)); //add delta
				}
				else
				{
					object->rotationalVelocityDelta->addDependent(new Delta<glm::vec3>(new RotDragProvider(object, 10), { new SpaceGameObjectRotationalVelocityTarget(object) }, {}, 10));
				}

				//move to strafe around ship
				auto perpendicular = glm::cross(towardsShip, glm::vec3(0, 0, 1)); //get the velocity vector, by finding the perpendicular vector using the cross product
				auto movement = ALIEN_ACCELERATION * glm::normalize(perpendicular); //rescale it
				object->velocityDelta->addDependent(new Delta<glm::vec3>(new Vec3Provider(movement), { new SpaceGameObjectVelocityTarget(object) }, {}, 5, ALIEN_MAX_VELOCITY)); //add delta

				//fire
				if (object->fireDelay == 0)
				{
					theta -= M_PI/2;
					object->fireDelay = ALIEN_FIRE_DELAY;
					auto alienVelocity = object->getVelocity();
					auto objectPosition = object->getPosition();
					TemporarySpaceGameObject* alienProjectile = new TemporarySpaceGameObject(
						alienProjectileModel,
						glm::vec3(
							objectPosition.x + (cos(theta) * 0.07), //second half moves the spawn point away from the center of the ship
							objectPosition.y + (sin(theta) * 0.07), //0.05 is the distance between the center and tip
							objectPosition.z
						),
						glm::vec3(
							alienVelocity.x + (cos(theta) * PROJECTILE_VELOCITY_MULT),
							alienVelocity.y + (sin(theta) * PROJECTILE_VELOCITY_MULT),
							alienVelocity.z + 0.0f
						),
						glm::vec3(0, 0, 0),
						glm::vec3(0.0f, 0.0f, 0.0f),
						glm::vec3(0.005f, 0.005f, 0.005f), //glm::vec3(0.005f, 0.005f, 0.005f),
						modelViewLocation,
						unordered_set<tag>{ ALIEN_PROJECTILE },
						PROJECTILE_DURATION
					);
					toAdd.push_back(alienProjectile);
					collisionHandler->Register(alienProjectile);
				}
			}
		}
	}
	for (auto object : toAdd)
	{
		objects.push_back(object);
	}
	
	//check for collisions
	auto collisions = collisionHandler->Check();
	for (auto it = collisions->begin(); it != collisions->end(); it++) //for each collision
	{
		bool firstIs[5] = {
			it->first->tags.count(SHIP) == 1,
			it->first->tags.count(SHIP_PROJECTILE) == 1,
			it->first->tags.count(ASTEROID) == 1,
			it->first->tags.count(ALIEN) == 1,
			it->first->tags.count(ALIEN_PROJECTILE) == 1
		};
		bool secondIs[5] = {
			it->second->tags.count(SHIP) == 1,
			it->second->tags.count(SHIP_PROJECTILE) == 1,
			it->second->tags.count(ASTEROID) == 1,
			it->second->tags.count(ALIEN) == 1,
			it->second->tags.count(ALIEN_PROJECTILE) == 1
		};
		//destroy both if it's a projectile and the other is not friendly
		if ((firstIs[SHIP_PROJECTILE] && !secondIs[SHIP]) || (secondIs[SHIP_PROJECTILE] && !firstIs[SHIP]))
		{
			it->first->markedForDelete = true;
			it->second->markedForDelete = true;
			if (firstIs[ALIEN] || secondIs[ALIEN])
			{
				newScore += SCORE_KILL_ALIEN;
			}
		}
		else if ((firstIs[ALIEN_PROJECTILE] && !secondIs[ALIEN]) || (secondIs[ALIEN_PROJECTILE] && !firstIs[ALIEN]))
		{
			it->first->markedForDelete = true;
			it->second->markedForDelete = true;
		}
		else { //bouncy collision

			//this current implemention conserves the magnitude of velocity. This is not a thing in real life.
			//Some guy on Reddit (src: trust me bro :skull:) says it is a thing if all masses are identical, and based on my tests: with this constraint in place energy is also conserved.

			//if the ship is in there, the system will always gain/lose whatever because the ship's magic (makes energy from controls, loses it from drag).

			SpaceGameObject* first = (SpaceGameObject*)(it->first);
			SpaceGameObject* second = (SpaceGameObject*)(it->second);

			glm::vec3 dir = glm::normalize(first->getPosition() - second->getPosition());
			constexpr float m1 = 1;
			constexpr float m2 = 1; //might add real mass later idk, will have to use a more realistic collision calculation because making this change stops energy from being conserved

			auto v = glm::length(first->getVelocity()) + glm::length(second->getVelocity());

			first->setVelocity((m2 / (m1 + m2))* v* dir);
			second->setVelocity((m1 / (m1 + m2))* v * -dir);

			//asteroids trade spin
			if (firstIs[ASTEROID] && secondIs[ASTEROID])
			{
				auto averageRotation = (first->getRotationalVelocity() + second->getRotationalVelocity()) * 0.5f;
				first->setRotationalVelocity(averageRotation);
				second->setRotationalVelocity(averageRotation);
			}
			else {
				if (firstIs[SHIP] || secondIs[SHIP])
				{
					ship->StunFor(SHIP_STUN_DURATION);
				}

				if (firstIs[ALIEN])
				{
					first->StunFor(ALIEN_STUN_DURATION);
				}
				if (secondIs[ALIEN])
				{
					second->StunFor(ALIEN_STUN_DURATION);
				}
			}
		}
	}
	delete collisions;
	//TODO update score counter
	if (newScore != score)
	{
		score = newScore;
		delete scoreValueTextBox;
		scoreValueTextBox = new TextBox(std::to_string(score), textureAtlasHandle, textureLocation2D, translationLocation2D, colorMaskLocation2D, SCORE_COUNTER_POSITION, SCORE_COUNTER_SIZE, windowDimensions);
	}

	if (score >= 1000000)
	{
		return SwitchState{ GAME_OVER, score };
	}

	//draw debug info
	if (showDebugInfo)
	{
		//draw the bounds of the quadtree, highlighting the node that the ship is in
		DrawQuadTree(true, true, true, ship, collisionHandler, texturedColoredShader, blockColorShader, colorLocation);
	}

	auto cameraPosition = glm::vec3(0,0,-5);// +;
	auto centerPosition = glm::vec3(0,0,0);
	glm::mat4 viewMatrix(glm::lookAt(cameraPosition, centerPosition, glm::vec3(0, 1, 0))); //calculate view matrix

	//draw text
	textShader2D->Use();
	scoreValueTextBox->Draw();

	//draw world objects
	texturedColoredShader->Use();
	for (size_t i = 0; i < objects.size(); i++)
	{
		auto target = objects.at(i);
		//Tick everything
		target->Tick();
		//draw all objects, deleting any that have been marked for delete
		target->Draw(viewMatrix);
		if (target->markedForDelete)
		{
			if (target->tags.count(ASTEROID) == 1)
			{
				numAsteroids--;
			}
			else if (target->tags.count(ALIEN) == 1)
			{
				numAliens--;
			}
			else if (target == ship) { //ship destroyed, game over
				return SwitchState{ GAME_OVER, score };
				//TODO display game over 
			}

			collisionHandler->Remove(target);
			objects.erase(objects.begin() + i);

			i--;
		}
	}
	return SwitchState{UNCHANGED,0};
}