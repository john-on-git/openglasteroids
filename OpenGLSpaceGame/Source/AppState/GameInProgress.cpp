#define _USE_MATH_DEFINES
#include "GameInProgress.hpp"
#include "MainMenu.hpp"
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
#include "../WorldObject/TemporarySpaceGameObject.hpp"
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


GameInProgress::GameInProgress(void (*SetState)(AppState*), bool keyPressed[360], std::map<std::string, Model*>* models, std::map<std::string, Renderer2D*>* renderer2Ds, GLuint colorLocation, GLuint modelViewLocation, Program* texturedColoredShader, Program* blockColorShader, Program* textShader2D)
{
	this->SetState = SetState;
	this->keyPressed = keyPressed;

	this->models = models;
	this->renderer2Ds = renderer2Ds;
	this->asteroidModel = models->at("asteroid");
	this->projectileModel = models->at("projectile");
	this->shipModel = models->at("ship");

	this->colorLocation = colorLocation;
	this->modelViewLocation = modelViewLocation;

	this->texturedColoredShader = texturedColoredShader;
	this->blockColorShader = blockColorShader;
	this->textShader2D = textShader2D;

	this->ftLibrary = ftLibrary;
	this->ftMainFont = ftMainFont;

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

	fireDelay = 0;
	controlsDisabled = 0;

	showDebugInfo = false;
	showDebugInfoToggleDelay = 0;
}
void GameInProgress::OnEntry()
{
	texturedColoredShader->Use();
}
void GameInProgress::Tick()
{
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
			pos = glm::vec3(-.95f * ARENA_W, rel, 0.0f); //left
			angle = 0 - (M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //right
			break;
		case 1:
			pos = glm::vec3(rel, -.95f * ARENA_H, 0.0f); //bottom
			angle = (M_PI / 2) - (M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //up
			break;
		case 2:
			pos = glm::vec3(.95f * ARENA_W, rel, 0.0f); //right
			angle = (M_PI)-(M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //left
			break;
		case 3:
			pos = glm::vec3(rel, .95f * ARENA_H, 0.0f); //top
			angle = (M_PI * 3 / 2) - (M_PI / 4) + ((float)rand()) / RAND_MAX * (M_PI / 2); //down
			break;
		}
		SpaceGameObject* asteroid = new SpaceGameObject(
			asteroidModel,
			pos,	//pos
			glm::vec3(ASTEROID_INIT_VEL * cos(angle), ASTEROID_INIT_VEL * sin(angle), 0.0f),	//vel
			glm::vec3(0.2f, 0.1f, 0.3f),	//rot vel
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
	if (fireDelay > 0) { fireDelay--; }
	if (controlsDisabled > 0)
	{
		controlsDisabled--;
		if (controlsDisabled % (FPS / 4) == 0)
		{
			if (shipModel->meshes[0]->colorMask == COLOR_NONE)
			{
				shipModel->meshes[0]->colorMask = COLOR_FLASH;
			}
			else
			{
				shipModel->meshes[0]->colorMask = COLOR_NONE;
			}
		}
	}
	else
	{
		shipModel->meshes[0]->colorMask = COLOR_NONE;
		//get player input
		auto rad = glm::radians(ship->getAngle().y-90);

		if (ship != nullptr && (keyPressed[GLFW_KEY_W] || keyPressed[GLFW_KEY_UP]))
		{
			ship->velocityDelta->addDependent(new Delta<glm::vec3>(new Vec3Provider(glm::vec3(SHIP_MOVERATE_MULT * cos(rad), SHIP_MOVERATE_MULT * sin(rad), 0.0f)), {new SpaceGameObjectVelocityTarget(ship)}, {}, 10, SHIP_MAX_VELOCITY));
		}
		else if (ship != nullptr && (keyPressed[GLFW_KEY_S] || keyPressed[GLFW_KEY_DOWN]))
		{
			ship->velocityDelta->addDependent(new Delta<glm::vec3>(new Vec3Provider(glm::vec3(-SHIP_MOVERATE_MULT * cos(rad), -SHIP_MOVERATE_MULT * sin(rad), 0.0f)), { new SpaceGameObjectVelocityTarget(ship) }, {}, 10, SHIP_MAX_VELOCITY));
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

		if (ship != nullptr && (keyPressed[GLFW_KEY_Z] || keyPressed[GLFW_KEY_SPACE]) && fireDelay == 0)
		{
			glm::vec3 shipVelocity = ship->getVelocity();
			TemporarySpaceGameObject* projectile = new TemporarySpaceGameObject(
				projectileModel,
				glm::vec3(
					ship->getPosition().x + (cos(rad) * 0.10), //second half moves the spawn point away from the center of the ship
					ship->getPosition().y + (sin(rad) * 0.10), //0.05 is the distance between the center and tip
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
				unordered_set<tag>{ PROJECTILE },
				PROJECTILE_DURATION
			);
			objects.push_back(projectile);
			collisionHandler->Register(projectile);

			fireDelay = FIRE_DELAY; //reset fire delay
		}
	}

	//toggle debug info
	if (showDebugInfoToggleDelay > 0) { showDebugInfoToggleDelay--; }
	if (keyPressed[GLFW_KEY_F5] && showDebugInfoToggleDelay == 0)
	{
		showDebugInfo = !showDebugInfo;
		showDebugInfoToggleDelay = SHOW_DEBUG_INFO_TOGGLE_DELAY;
	}

	for (auto object : objects)
	{
		//toroidal space
		auto objectPosition = object->getPosition();
		if (objectPosition.x > ARENA_W) //off right
		{
			objectPosition.x = -ARENA_W;
		}
		else if (objectPosition.x < -ARENA_W) //off left
		{
			objectPosition.x = ARENA_W;
		}

		if (objectPosition.y < -ARENA_H) //off top
		{
			objectPosition.y = ARENA_W;
		}
		else if (objectPosition.y > ARENA_H) //off bottom
		{
			objectPosition.y = -ARENA_W;
		}
		object->setPosition(objectPosition);
	}

	//check for collisions
	auto collisions = collisionHandler->Check();
	for (auto it = collisions->begin(); it != collisions->end(); it++) //for each collision
	{
		//destroy both if it's a projectile
		if (it->first->tags.count(PROJECTILE) == 1 || it->second->tags.count(PROJECTILE) == 1)
		{
			it->first->markedForDelete = true;
			it->second->markedForDelete = true;
			if (it->first->tags.count(ASTEROID) == 1)
			{
				numAsteroids--;
			}
			if (it->second->tags.count(ASTEROID) == 1)
			{
				numAsteroids--;
			}
		}
		else { //generic elastic collision, trades spin

			//alright, so my lack of understanding of classical physics is coming back to bite me in the ass
			//this current implemention conserves the magnitude of velocity. This is not a thing in real life.
			//Some guy on Reddit says it is a thing if all masses are identical, and based on my tests: with this constraint in place energy is also conserved.

			//if the ship is in there, the system will always gain/lose whatever because the ship's magic (makes energy from controls, loses it from drag).

			SpaceGameObject* first = (SpaceGameObject*)(it->first);
			SpaceGameObject* second = (SpaceGameObject*)(it->second);

			glm::vec3 dir = glm::normalize(first->getPosition() - second->getPosition());
			float m1 = 1;
			float m2 = 1; //might add real mass later idk, will have to use a more realistic collision calculation because making this change stops energy from being conserved

			auto v = glm::length(first->getVelocity()) + glm::length(second->getVelocity());

			first->setVelocity((m2 / (m1 + m2))* v* dir);
			second->setVelocity((m1 / (m1 + m2))* v * -dir);

			auto averageRotation = (first->getRotationalVelocity() + second->getRotationalVelocity()) * 0.5f;
			first->setRotationalVelocity(averageRotation);
			second->setRotationalVelocity(averageRotation);

			if (first->tags.count(SHIP) == 1 || second->tags.count(SHIP) == 1)
			{
				controlsDisabled = SHIP_STUN_DURATION;
			}
		}
	}
	delete collisions;

	//draw debug info
	if (showDebugInfo)
	{
		//draw the bounds of the quadtree, highlighting the node that the ship is in
		DrawQuadTree(true, true, true, ship, collisionHandler, texturedColoredShader, blockColorShader, colorLocation);
	}

	auto cameraPosition = glm::vec3(0,0,-5);// +;
	auto centerPosition = glm::vec3(0,0,0);
	glm::mat4 viewMatrix(glm::lookAt(cameraPosition, centerPosition, glm::vec3(0, 1, 0))); //calculate view matrix

	for (size_t i = 0; i < objects.size(); i++)
	{
		//Tick() everything
		objects.at(i)->Tick();
		//draw all objects, deleting any that have been marked for delete
		objects.at(i)->Draw(viewMatrix);
		if (objects.at(i)->markedForDelete)
		{
			collisionHandler->Remove(objects.at(i));

			if (objects.at(i) == ship) { //ship destroyed, game over
				SetState(new MainMenu(SetState, keyPressed, models, renderer2Ds, colorLocation, modelViewLocation, texturedColoredShader, blockColorShader, textShader2D)); //TODO display a game over message & kick back to main menu
				//TODO display game over menu
				return;
			}
			objects.erase(objects.begin() + i);
			i--;
		}
	}
}