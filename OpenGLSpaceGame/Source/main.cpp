#include <iostream>
#include <fstream>
#include <windows.h>
#include <cmath>
#include <vector>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <conio.h>
#include <assimp/Importer.hpp>
#include <assimp\postprocess.h>

#include "Program/program.hpp"
#include "BufferedAiMesh/BufferedAiMesh.hpp"
#include "WorldObject/SpaceGameObject.hpp"
#include "WorldObject/TemporarySpaceGameObject.hpp"
#include "Texture/Texture.hpp"
#include "main.hpp"
#include "QuadTreeCollisionHandler/QuadTreeCollisionHandler.hpp"
#include "Delta/Delta.hpp"
#include "Delta/DeltaProvider.hpp"
#include "Delta/DeltaTarget.hpp"
#include "Delta/DeltaProviders/Vec3Provider.hpp"
#include "Delta/DeltaTargets/Vec3Target.hpp"
#include "Delta/DeltaProviders/DragProvider.hpp"
#include "Delta/DeltaTargets/WorldObjectAngleTarget.hpp"
#include "Delta/DeltaTargets/WorldObjectPositionTarget.hpp"


using namespace std;

//Debug logic for visualizing the quadtree. Not very efficient but idc.
static void DrawQuadTree(bool drawAllRegions, bool drawShipRegion, bool drawShipOABB, WorldObject* ship, QuadTreeCollisionHandler* collisionHandler, Program* texturedColoredShader, Program* blockColorShader, GLuint colorLocation) {
	blockColorShader->Use();
	int STRIDE = 2;
	size_t COORDS_LEN = 8;
	size_t INDICES_LEN = 8;
	glad_glUniform4fv(colorLocation, 1, glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	unsigned int vertIndices[] {0, 1, 1, 2, 2, 3, 3, 0};
	if (drawAllRegions) {
		glad_glLineWidth(1);
		vector<glm::vec2*> flattened;
		collisionHandler->GetAllBounds(&flattened);
		while (!flattened.empty()) {
			glm::vec2* bounds =flattened.back();
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

bool keyPressed[360];

int main()
{
	//initialize glfw
	if (!glfwInit())
	{
		OutputDebugStringW(L"FATAL: failed to initialize glfw\n");
		exit(1);
	}
	//initialize window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
	if (window == nullptr)
	{
		OutputDebugStringW(L"FATAL: failed to initialize glfw window\n");
		exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(1);
	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		OutputDebugStringW(L"FATAL: failed to initialize glad\n");
		exit(1);
	}
	//glEnables
	glad_glEnable(GL_DEPTH_TEST);
	glad_glEnable(GL_TEXTURE_2D);
	glad_glEnable(GL_BLEND);
	glad_glBlendEquation(GL_FUNC_ADD); // this is default
	glad_glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//gl drawing config
	glad_glClearColor(0.0f, 0.0f, 0.0f, 1);
	glad_glPointSize(5.0f);

	//shader setup
	//compile
	Program texturedColoredShader(
		"Shaders/TexturedColoredTransform/texturedColoredTransform.frag",
		"Shaders/TexturedColoredTransform/texturedColoredTransform.vert"
	);
	Program blockColorShader(
		"Shaders/BlockColor/blockColor.frag",
		"Shaders/BlockColor/blockColor.vert"
	);
	//get uniform locations
	blockColorShader.Use();
	GLuint colorLocation = glad_glGetUniformLocation(blockColorShader.handle, "color");

	texturedColoredShader.Use();
	GLuint
		textureLocation		= glad_glGetUniformLocation(texturedColoredShader.handle, "tex"),
		colorMaskLocation	= glad_glGetUniformLocation(texturedColoredShader.handle, "colorMask"),
		projectionLocation	= glad_glGetUniformLocation(texturedColoredShader.handle, "projection"),
		viewLocation		= glad_glGetUniformLocation(texturedColoredShader.handle, "view"),
		modelLocation		= glad_glGetUniformLocation(texturedColoredShader.handle, "model");
	//check for failure
	if (projectionLocation == -1 || viewLocation == -1 || modelLocation == -1)
	{
		OutputDebugStringW(L"FATAL: couldn't get uniform location\n");
		exit(1);
	}

	//textures
	auto shipTex = Texture("textures/ship.png");
	auto projectileTex = Texture("textures/projectile.png");

	//model
	auto shipModel = Model(
		"Models/ship.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ shipTex.handle },
		std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
		1
	);
	auto projectileModel = Model(
		"Models/sphere.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ projectileTex.handle },
		std::vector<glm::vec4>{ glm::vec4(2, 2, 2, 1) },
		1
	);
	auto cubeModel = Model(
		"Models/cube.obj",
		textureLocation,
		colorMaskLocation,
		std::vector<GLuint>{ shipTex.handle },
		std::vector<glm::vec4>{ glm::vec4(1, 1, 1, 1) },
		1
	);

	//set up world stuff
	auto ship = new SpaceGameObject(
		&shipModel,
		glm::vec3(0.0f, 0.0f, -5.0f),	//pos
		glm::vec3(0.0f, 0.0f, 0.0f),	//vel
		glm::vec3(0.0f, 0.0f, 0.0f),	//rot vel
		glm::vec3(270.0f, 50.0f, 0.0f),	//rot
		glm::vec3(0.05f, 0.05f, 0.05f), //scale
		projectionLocation,
		viewLocation,
		modelLocation,
		unordered_set<tag>{ SHIP }
	);
	ship->velocityDelta->addDependent(new Delta<glm::vec3>(new DragProvider(&ship->velocity), { new Vec3Target(&ship->velocity) }, {}));
	ship->rotationalVelocityDelta->addDependent(new Delta<glm::vec3>(new DragProvider(&ship->rotationalVelocity, 10), { new Vec3Target(&ship->rotationalVelocity) }, {}));


	auto dummy1 = new SpaceGameObject(
		&cubeModel,
		glm::vec3(0.4f, 0.4f, -5.0f),	//pos
		glm::vec3(0.0f, 0.0f, 0.0f),	//vel
		glm::vec3(0.2f, 0.1f, 0.3f),	//rot vel
		glm::vec3(270.0f, 0.0f, 0.0f),	//rot
		glm::vec3(0.05f, 0.05f, 0.05f),	//scale
		projectionLocation,
		viewLocation,
		modelLocation,
		unordered_set<tag>{}
	);
	auto dummy2 = new SpaceGameObject(
		&cubeModel,
		glm::vec3(-0.4f, -0.4f, -5.0f),	//pos
		glm::vec3(0.0f, 0.0f, 0.0f),	//vel
		glm::vec3(0.2f, 0.1f, 0.3f),	//rot vel
		glm::vec3(270.0f, 0.0f, 0.0f),	//rot
		glm::vec3(0.05f, 0.05f, 0.05f),	//scale
		projectionLocation,
		viewLocation,
		modelLocation,
		unordered_set<tag>{}
	);
	auto dummy3 = new SpaceGameObject(
		&cubeModel,
		glm::vec3(-0.4f, -0.4f, -5.0f),	//pos
		glm::vec3(0.0f, 0.0f, 0.0f),	//vel
		glm::vec3(0.2f, 0.1f, 0.3f),	//rot vel
		glm::vec3(270.0f, 0.0f, 0.0f),	//rot
		glm::vec3(0.05f, 0.05f, 0.05f),	//scale
		projectionLocation,
		viewLocation,
		modelLocation,
		unordered_set<tag>{}
	);
	auto dummy4 = new SpaceGameObject(
		&cubeModel,
		glm::vec3(-0.4f, -0.4f, -5.0f),	//pos
		glm::vec3(0.0f, 0.0f, 0.0f),	//vel
		glm::vec3(0.2f, 0.1f, 0.3f),	//rot vel
		glm::vec3(270.0f, 0.0f, 0.0f),	//rot
		glm::vec3(0.05f, 0.05f, 0.05f),	//scale
		projectionLocation,
		viewLocation,
		modelLocation,
		unordered_set<tag>{}
	);
	auto dummy5 = new SpaceGameObject(
		&cubeModel,
		glm::vec3(-0.4f, -0.4f, -5.0f),	//pos
		glm::vec3(0.0f, 0.0f, 0.0f),	//vel
		glm::vec3(0.2f, 0.1f, 0.3f),	//rot vel
		glm::vec3(270.0f, 0.0f, 0.0f),	//rot
		glm::vec3(0.05f, 0.05f, 0.05f),	//scale
		projectionLocation,
		viewLocation,
		modelLocation,
		unordered_set<tag>{}
	);

	vector<SpaceGameObject*> objects = {
		ship,
		dummy1,
		dummy2,
		dummy3,
		dummy4,
		dummy5
	};

	//game stuff
	QuadTreeCollisionHandler collisionHandler(
		10,
		glm::vec2(-10.1f, -10.1f),
		glm::vec2( 10.1f,  10.1f)
	);
	for (auto it = objects.begin(); it != objects.end(); it++) //register all objects
	{
		collisionHandler.Register(*it);
	}
	//set up deltas stuff
	auto shipAngleTarget = WorldObjectAngleTarget(ship);

	unsigned char fireDelay = 0;

	bool showDebugInfo = false;
	unsigned char showDebugInfoToggleDelay = 0;

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		if (fireDelay > 0)
			fireDelay--;
		if (showDebugInfoToggleDelay > 0)
			showDebugInfoToggleDelay--;
		//get player input
		auto rad = glm::radians(ship->getAngle().y);
		if (ship != nullptr && keyPressed[GLFW_KEY_W] || keyPressed[GLFW_KEY_UP])
		{
			auto move = glm::vec3(
				SHIP_MOVERATE_MULT * sin(rad),
				SHIP_MOVERATE_MULT * cos(rad),
				0.0f
			);
			ship->velocityDelta->addDependent(new Delta<glm::vec3>(new Vec3Provider(&move), { new Vec3Target(&ship->velocity) }, {}, 10, SHIP_MAX_VELOCITY));
		}
		else if (ship != nullptr && keyPressed[GLFW_KEY_S] || keyPressed[GLFW_KEY_DOWN])
		{
			auto move = glm::vec3(
				-SHIP_MOVERATE_MULT * sin(rad),
				-SHIP_MOVERATE_MULT * cos(rad),
				0.0f
			);
			ship->velocityDelta->addDependent(new Delta<glm::vec3>(new Vec3Provider(&move), { new Vec3Target(&ship->velocity) }, {}, 10, SHIP_MAX_VELOCITY));
		}

		if (ship != nullptr && keyPressed[GLFW_KEY_A] || keyPressed[GLFW_KEY_LEFT])
		{
			ship->rotationalVelocityDelta->addDependent(new Delta<glm::vec3>(
				new Vec3Provider(new glm::vec3(0.0f, -SHIP_TURNRATE_MULT, 0.0f)),
				{ new Vec3Target(&ship->rotationalVelocity) },
				{},
				1
			));
		}
		else if (ship != nullptr && keyPressed[GLFW_KEY_D] || keyPressed[GLFW_KEY_RIGHT])
		{
			ship->rotationalVelocityDelta->addDependent(new Delta<glm::vec3>(
				new Vec3Provider(new glm::vec3(0.0f, SHIP_TURNRATE_MULT, 0.0f)), 
				{ new Vec3Target(&ship->rotationalVelocity) },
				{},
				1
			));
		}

		if (ship!=nullptr && (keyPressed[GLFW_KEY_Z] || keyPressed[GLFW_KEY_SPACE]) && fireDelay == 0)
		{
			auto projectile = new TemporarySpaceGameObject(
				&projectileModel,
				glm::vec3(
					ship->getPosition().x + (sin(rad) * 0.10), //second half moves the spawn point away from the center of the ship
					ship->getPosition().y + (cos(rad) * 0.10), //0.05 is the distance between the center and tip
					ship->getPosition().z
				),
				glm::vec3(
					ship->velocity.x + (sin(rad) * PROJECTILE_VELOCITY_MULT),
					ship->velocity.y + (cos(rad) * PROJECTILE_VELOCITY_MULT),
					ship->velocity.z + 0.0f
				),
				glm::vec3(0,0,0),
				glm::vec3(270.0f, 0.0f, 0.0f),
				glm::vec3(0.005f, 0.005f, 0.005f),
				projectionLocation,
				viewLocation,
				modelLocation,
				unordered_set<tag>{ PROJECTILE },
				PROJECTILE_DURATION
			);
			objects.push_back(projectile);
			collisionHandler.Register(projectile);

			fireDelay = FIRE_DELAY; //reset fire delay
		}

		//toggle debug info
		if (keyPressed[GLFW_KEY_F5] && showDebugInfoToggleDelay==0)
		{
			showDebugInfo = !showDebugInfo; //no Intellisense, bitwise ~ was not intended
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

			object->model->meshes[0].colorMask = glm::vec4(1, 1, 1, 1); //reset color
		}

		//check for collisions
			auto collisions = collisionHandler.Check();
			for (auto it = collisions->begin(); it != collisions->end(); it++) //for each collision
			{
				//destroy both if it's a projectile
				if (it->first->tags.count(PROJECTILE)==1 || it->second->tags.count(PROJECTILE)==1)
				{
					it->first->markedForDelete = true;
					it->second->markedForDelete = true;
				}
				else { //generic collision, knocks both things away and trades spin
					
					SpaceGameObject* first = (SpaceGameObject*)(it->first);
					SpaceGameObject* second = (SpaceGameObject*)(it->second);

					first->velocity = glm::vec3(COLLIDE_BOUNCE_STRENGTH * (it->first->getPosition() - it->second->getPosition()));
					second->velocity = glm::vec3(COLLIDE_BOUNCE_STRENGTH * (it->second->getPosition() - it->first->getPosition()));

					auto averageRotation = (first->rotationalVelocity + second->rotationalVelocity) * 0.5f;
					first->rotationalVelocity = averageRotation;
					second->rotationalVelocity = averageRotation;
				}
			}
			delete collisions;
		//clear framebuffers
			glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//draw debug info
		if (showDebugInfo)
		{
			//draw the bounds of the quadtree, highlighting the node that the ship is in
			DrawQuadTree(true, true, false, ship, &collisionHandler, &texturedColoredShader, &blockColorShader, colorLocation);
		}

		for (size_t i = 0; i < objects.size(); i++)
		{
			//Tick() everything
			objects.at(i)->Tick();
			//draw all objects, deleting any that have been marked for delete
			objects.at(i)->Draw();
			if (objects.at(i)->markedForDelete)
			{
				collisionHandler.Remove(objects.at(i));

				if (objects.at(i) == ship) { //ship destroyed, game over
					exit(0); //TODO display a game over message & kick back to main menu
				}
				objects.erase(objects.begin() + i);
				i--;
			}
		}
		//check for GL errors
			GLenum err = glad_glGetError();
			while(err!=GL_NO_ERROR)
			{
				cout << "\nGL Error: " << to_string(err);
				err = glad_glGetError();
			}
		//glfw stuff
			glfwSwapBuffers(window);
			glfwPollEvents();
		Sleep(FRAME_DELAY);
	}

	//free resources
		//glfw
		glfwDestroyWindow(window);
		glfwTerminate();
		//vertex buffer
		//vertex array
}

/// <summary>
/// called whenever a key is pressed
/// </summary>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		keyPressed[key] = true;
	}
	else if (action==GLFW_RELEASE)
	{
		keyPressed[key] = false;
	}
}