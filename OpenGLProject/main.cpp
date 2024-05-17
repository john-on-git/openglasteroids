#include <iostream>
#include <fstream>
#include <windows.h>
#include <cmath>
#include <vector>
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

#include "Class/Program/program.hpp"
#include "Class/BufferedAiMesh/BufferedAiMesh.hpp"
#include "Class/WorldObject/WorldObject.hpp"
#include "Class/Texture/Texture.hpp"
#include "main.hpp"
#include "Class/Delta/Delta.hpp"
#include "tag.hpp"
#include "Class/QuadTreeCollisionHandler/QuadTreeCollisionHandler.hpp"


using namespace std;

//Debug logic for visualizing the quadtree. MASSIVE MEMORY LEAK! sort of dumped this here, should be on quadtreecollisionhandler I suppose.
void drawQuadTree(bool drawAllRegions, bool drawShipRegion, WorldObject* ship, QuadTreeCollisionHandler* collisionHandler, Program* texturedColoredShader, Program* lineShader) {
	if (drawAllRegions) {
		glad_glLineWidth(1);
		vector<glm::vec2**> flattened;
		collisionHandler->GetAllBounds(&flattened);
		while (!flattened.empty()) {
			glm::vec2* bounds = *(flattened.back());
			//set up line quadtree debugger
			int STRIDE = 4;
			size_t COORDS_LEN = 16;
			size_t INDICES_LEN = 8;
			float* verts = new float[] {
				bounds[0].x, bounds[1].y, -1.0f, 1.0f, //topleft
				bounds[1].x, bounds[1].y, -1.0f, 1.0f, //topright
				bounds[1].x, bounds[0].y, -1.0f, 1.0f, //bottomright
				bounds[0].x, bounds[0].y, -1.0f, 1.0f, //bottomleft
			};
			unsigned int* vertIndices = new unsigned int[] {0,1, 1,2, 2,3, 3, 0};
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

			delete[] verts;
			delete[] vertIndices;
			//set vertex attrib pointers OK
			glad_glEnableVertexAttribArray(0);
			glad_glVertexAttribPointer( //position
				0,
				4,
				GL_FLOAT,
				GL_FALSE,
				STRIDE * sizeof(float),
				NULL
			);

			lineShader->Use();
			glad_glBindVertexArray(quadtreeVAO);
			glad_glDrawElements(
				GL_LINES,
				INDICES_LEN,
				GL_UNSIGNED_INT,
				NULL
			);
			texturedColoredShader->Use();
			flattened.pop_back();
		}
	}
	if (drawShipRegion) {
		glad_glLineWidth(10);
		//set up line quadtree debugger
		auto bounds = collisionHandler->GetNodeBoundsForObject(ship);
		int STRIDE = 4;
		size_t COORDS_LEN = 16;
		size_t INDICES_LEN = 8;
		float* verts = new float[] {
			bounds[0].x, bounds[1].y, -1.0f, 1.0f, //topleft
			bounds[1].x, bounds[1].y, -1.0f, 1.0f, //topright
			bounds[1].x, bounds[0].y, -1.0f, 1.0f, //bottomright
			bounds[0].x, bounds[0].y, -1.0f, 1.0f, //bottomleft
		};
		unsigned int* vertIndices = new unsigned int[] {0,1, 1,2, 2,3, 3,0};

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

		delete[] verts;
		delete[] vertIndices;

		//set vertex attrib pointers OK
		glad_glEnableVertexAttribArray(0);
		glad_glVertexAttribPointer( //position
			0,
			4,
			GL_FLOAT,
			GL_FALSE,
			STRIDE * sizeof(float),
			NULL
		);

		lineShader->Use();
		glad_glBindVertexArray(quadtreeVAO);
		glad_glDrawElements(
			GL_LINES,
			INDICES_LEN,
			GL_UNSIGNED_INT,
			NULL
		);
		texturedColoredShader->Use();
	}
	lineShader->Use();
	glad_glLineWidth(5);
	//set up line quadtree debugger
	glm::vec3* bounds = ship->getBoundingBox();
	int STRIDE = 4;
	size_t COORDS_LEN = 16;
	size_t INDICES_LEN = 8;
	float* verts = new float[] {
		bounds[0].x, bounds[0].y, -1, 1.0f, //topleft
		bounds[1].x, bounds[1].y, -1, 1.0f, //topright
		bounds[5].x, bounds[5].y, -1, 1.0f, //bottomright
		bounds[4].x, bounds[4].y, -1, 1.0f, //bottomleft
	};
	unsigned int* vertIndices = new unsigned int[] {0, 1, 1, 2, 2, 3, 3, 0};
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

	delete[] verts;
	delete[] vertIndices;

	//set vertex attrib pointers OK
	glad_glEnableVertexAttribArray(0);
	glad_glVertexAttribPointer( //position
		0,
		4,
		GL_FLOAT,
		GL_FALSE,
		STRIDE * sizeof(float),
		NULL
	);

	lineShader->Use();
	glad_glBindVertexArray(quadtreeVAO);
	glad_glDrawElements(
		GL_LINES,
		INDICES_LEN,
		GL_UNSIGNED_INT,
		NULL
	);
	texturedColoredShader->Use();
}

/*	TODO TODO TODO TODO TODO	*\
	texture support			 ✓
	loading models from file ✓
	licensing stuff for libs ✓
	fix memory leak			 ✓
	fix bullet shooting		 
	collision detection		
\*								*/

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
		GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
		if (window == NULL)
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
			Program lineShader(
				"Shaders/MyFirstShader/myFirst.frag",
				"Shaders/MyFirstShader/myFirst.vert"
			);
		//get uniform locations
			texturedColoredShader.Use();
			GLuint
				textureLocation		= glad_glGetUniformLocation(texturedColoredShader.handle, "tex"),
				colorLocation		= glad_glGetUniformLocation(texturedColoredShader.handle, "colorMask"),
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
		auto shipTex = new Texture("textures/ship.png");
		auto projectileTex = new Texture("textures/projectile.png");
	//model
		auto shipModel = new Model(
			"Models/ship.obj",
			textureLocation,
			colorLocation,
			new GLuint[]{ shipTex->handle },
			new glm::vec4[]{ glm::vec4(1,1,1,1) },
			1
		);
		auto projectileModel = new Model(
			"Models/sphere.obj",
			textureLocation,
			colorLocation,
			new GLuint[]{ projectileTex->handle },
			new glm::vec4[]{ glm::vec4(2,2,2,1) },
			1
		);
	//set up world stuff
		auto ship = WorldObject(
			shipModel,
			glm::vec3(0.0f, 0.0f, -5.0f),//pos
			glm::vec3(270.0f, 45.1f, 0.0f),
			glm::vec3(0.05f, 0.05f, 0.05f), //scale
			projectionLocation,
			viewLocation,
			modelLocation,
			new vector<tag>{ SHIP }
		);
		auto dummy = WorldObject(
			projectileModel,
			glm::vec3(-0.4f, -0.4f, -5.0f),//pos
			glm::vec3(270.0f, 0.0f, 0.0f),
			glm::vec3(0.05f, 0.05f, 0.05f), //scale
			projectionLocation,
			viewLocation,
			modelLocation,
			new vector<tag>{}
		);
		auto objects = vector<WorldObject*>{
				&ship,
				&dummy
		};
	//game stuff
		QuadTreeCollisionHandler collisionHandler(
			5,
			new glm::vec2[]
			{
				glm::vec2( 1.1f, 1.1f),
				glm::vec2(-1.1f,-1.1f)
			}
		);
		vector<Delta*> deltas;
		vector<WorldObject*> projectiles;
		auto shipVelocity = Delta(
			&ship.position,
			new glm::vec3(0.0f, 0.0f, 0.0f)
		);
		deltas.push_back(&shipVelocity);
		auto shipDrag = Delta(
			&shipVelocity,
			new glm::vec3(
				-SHIP_DRAG_COEFFICIENT * shipVelocity.magnitude->x,
				-SHIP_DRAG_COEFFICIENT * shipVelocity.magnitude->y,
				-SHIP_DRAG_COEFFICIENT * shipVelocity.magnitude->z
			),
			SHIP_MAX_VELOCITY,
			-1
		);
		deltas.push_back(&shipDrag);
		unsigned char fireDelay = 0;
	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//manually update ship drag (TODO this sort of behaviour should be generalised & encapsulated in Delta)
		shipDrag.magnitude->x = -SHIP_DRAG_COEFFICIENT * shipVelocity.magnitude->x;
		shipDrag.magnitude->y = -SHIP_DRAG_COEFFICIENT * shipVelocity.magnitude->y;
		shipDrag.magnitude->z = -SHIP_DRAG_COEFFICIENT * shipVelocity.magnitude->z;

		if (fireDelay > 0)
			fireDelay--;
		//get player input
		auto rad = glm::radians(ship.angle.y);
		if (keyPressed[GLFW_KEY_W])
		{
			deltas.push_back(new Delta(
				&shipVelocity,
				new glm::vec3(
					SHIP_MOVERATE_MULT * sin(rad),
					SHIP_MOVERATE_MULT * cos(rad),
					0.0f
				),
				SHIP_MAX_VELOCITY,
				10
			));
		}
		else if (keyPressed[GLFW_KEY_S])
		{
			deltas.push_back(new Delta(
				&shipVelocity,
				new glm::vec3(
					-1 * SHIP_MOVERATE_MULT * sin(rad),
					-1 * SHIP_MOVERATE_MULT * cos(rad),
					0.0f
				),
				10
			));
		}

		if (keyPressed[GLFW_KEY_A])
		{
			deltas.push_back(new Delta(
				&ship.angle,
				new glm::vec3(0.0f, SHIP_TURNRATE_MULT * -1, 0.0f),
				10
			));
		}
		else if (keyPressed[GLFW_KEY_D])
		{
			deltas.push_back(new Delta(
				&ship.angle,
				new glm::vec3(0.0f, SHIP_TURNRATE_MULT, 0.0f),
				10
			));
		}

		if (keyPressed[GLFW_KEY_SPACE] && fireDelay==0)
		{
			auto projectile = new WorldObject(
				projectileModel,
				glm::vec3(
					ship.position.x + (sin(rad) * 0.05), //second half moves the spawn point away from the center of the ship
					ship.position.y + (cos(rad) * 0.05), //0.05 is the distance between the center and tip
					ship.position.z
				),
				glm::vec3(270.0f, 0.0f, 0.0f),
				glm::vec3(0.005f, 0.005f, 0.005f),
				projectionLocation,
				viewLocation,
				modelLocation,
				new vector<tag>{ PROJECTILE }
			);
			objects.push_back(projectile);
			projectiles.push_back(projectile);
			deltas.push_back(new Delta(
				&(projectile->position),
				new glm::vec3(
					shipVelocity.magnitude->x + (sin(rad) * BULLET_VELOCITY_MULT),
					shipVelocity.magnitude->y + (cos(rad) * BULLET_VELOCITY_MULT),
					shipVelocity.magnitude->z + 0.0f
				)
			));
			fireDelay = FIRE_DELAY;
		}
		
		for (auto object : objects)
		{
			//toroidal space
				if (object->position.x > ARENA_W) //off left
				{
					object->position.x = -ARENA_W;
				}
				else if (object->position.x < -ARENA_W) //off right
				{
					object->position.x = ARENA_W;
				}

				if (object->position.y < -ARENA_H) //off top
				{
					object->position.y = ARENA_W;
				}
				else if (object->position.y > ARENA_H) //off bottom
				{
					object->position.y = -ARENA_W;
				}

			object->model->meshes[0].colorMask = glm::vec4(1, 1, 1, 1); //reset color
		}

		//check for collisions
			collisionHandler.Update(objects); //spatially partition all objects
			//check collision
			auto broadCollisions = collisionHandler.GetBroadCollisions();
			for (auto it = broadCollisions->begin(); it != broadCollisions->end();it++) //for each collision
				if (collisionHandler.GetFineCollision(it->first, it->second))
				{
					//color change for collision debugging
					it->first->model->meshes[0].colorMask = glm::vec4(2, 1, 1, 1);
					it->second->model->meshes[0].colorMask = glm::vec4(2, 1, 1, 1);
				}
		//clear framebuffers
			glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		delete broadCollisions;
		//draw the bounds of the quadtree, highlighting the node that the ship is in
		drawQuadTree(false, false, &ship, &collisionHandler, &texturedColoredShader, &lineShader);
		//draw all objects, deleting any that have been marked for delete
			for (auto it = objects.begin(); it != objects.end();)
			{
				(*it)->Draw();
				if ((*it)->markedForDelete)
				{
					delete *it;
					it = objects.erase(it);
				}
				else
					it++;
			}
		//apply all deltas, deleting any that have expired
			for (auto it = deltas.begin(); it != deltas.end();)
				if ((*it)->Tick())
				{
					delete *it;
					it = deltas.erase(it);
				}
				else 
					it++;
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