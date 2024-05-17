#pragma once

#include <iostream>
#include "../ICollisionHandler/ICollisionHandler.hpp"
#include <glm/ext/vector_float2.hpp>


class QuadTreeCollisionHandler : public ICollisionHandler {
	public:
		QuadTreeCollisionHandler(unsigned char maxDepth, glm::vec2* initialBounds);
		virtual void Update(vector<WorldObject*> v);
		virtual unordered_set<UnorderedPair<WorldObject*>>* GetBroadCollisions();
		virtual bool GetFineCollision(WorldObject* a, WorldObject* b);
		virtual glm::vec2* GetNodeBoundsForObject(WorldObject* object);
		vector<glm::vec2*>* GetAllBounds();
	private:
		glm::vec2* initialBounds;
		unsigned char maxDepth;
		//15.5.2024, why is this defined in a header file?
		class qnode {
			public:
				vector<WorldObject*> contents;
				qnode* parent;
				qnode* children[4];
				glm::vec2 bounds[2];
				qnode(qnode* parent, glm::vec2 bottomLeft, glm::vec2 topRight)
				{
					this->parent = parent;

					children[0] = NULL;
					children[1] = NULL;
					children[2] = NULL;
					children[3] = NULL;

					this->bounds[0] = bottomLeft;
					this->bounds[1] = topRight;
				}

				~qnode()
				{
					for (qnode* child : children) {
						delete child;
					}
				}
				
				/// <summary>
				/// insert an object into this tree
				/// </summary>
				/// <param name="obj">the object to insert</param>
				/// <returns>whether the operation was successful</returns>
				bool TryInsert(unsigned char maxDepth, unsigned char currentDepth, WorldObject* obj)
				{
					if (currentDepth >= maxDepth)
						return false;
					else
					{
						if (WillFit(obj)) //if the object can fit in this node
						{
							//try to insert in the subtree first
							bool inSubTree = false; //true if the object has been inserted somewhere in the subtree
							if (children[0] == NULL) //if this is a leaf node, create the children
							{
								children[0] = new qnode( //top left
									this,
									glm::vec2( //bottom left
										this->bounds[0].x,
										(this->bounds[0].y + this->bounds[1].y) / 2
									),
									glm::vec2( //top right
										(this->bounds[0].x + this->bounds[1].x) / 2,
										this->bounds[1].y
									)
								);
								children[1] = new qnode( //top right
									this,
									glm::vec2( //bottom left
										(this->bounds[0].x + this->bounds[1].x) / 2,
										(this->bounds[0].y + this->bounds[1].y) / 2
									),
									glm::vec2( //top right
										this->bounds[1].x,
										this->bounds[1].y
									)
								);
								children[2] = new qnode( //bottom left
									this,
									glm::vec2( //bottom left
										this->bounds[0].x,
										this->bounds[0].y
									),
									glm::vec2( //top right
										(this->bounds[0].x + this->bounds[1].x) / 2,
										(this->bounds[0].y + this->bounds[1].y) / 2
									)
								);
								children[3] = new qnode( //bottom right
									this,
									glm::vec2( //bottom left
										(this->bounds[0].x + this->bounds[1].x) / 2,
										this->bounds[0].y
									),
									glm::vec2( //top right
										this->bounds[1].x,
										(this->bounds[0].y + this->bounds[1].y) / 2
									)
								);
							}
							int i = 0;
							while (i<4 && !inSubTree) {
								inSubTree = inSubTree || children[i]->TryInsert(maxDepth, currentDepth + 1, obj);
								i++;
							}
							//if it did not fit anywhere in the subtree, put it in this node
							if (!inSubTree)
							{
								contents.push_back(obj);
							}
							return true;
						}
						else {
							return false; //failed to insert
						}
					}
				}
				qnode* Find(WorldObject* obj)
				{
					for (auto item : contents) //check this
						if (item == obj)
							return this;
					if(children[0]!=NULL) //if not a leaf, check children
						for (auto child : children)
						{
							auto res = child->Find(obj);
							if (res != NULL)
								return res;
						}
					return NULL; //not in tree
				}
				void DepthFirstFlatten(vector<glm::vec2*>* collector)
				{
					collector->push_back(this->bounds);
					for (auto child : children)
					{
						if (child != NULL) {
							child->DepthFirstFlatten(collector);
						}
					}
				}
			protected:
				bool WillFit(WorldObject* obj)
				{
					//calculate world coordinates of object's bounding box
					glm::vec3 objCoords3 = glm::vec3(obj->position.x, obj->position.y, obj->position.z);
					glm::vec3 absCoords[]{
						(obj->model->bBox[0] * obj->scale - objCoords3),
						(obj->model->bBox[1] * obj->scale - objCoords3)
					};
					//determine whether the object is located fully inside this region
					return	(bounds[0].x >= absCoords[0].x) && (bounds[0].y >= absCoords[0].y) && //bottom left
							(bounds[1].x <= absCoords[1].x) && (bounds[1].y <= absCoords[1].y); //top right
				}
		};
		qnode* root; //here instead of at the top as qnode needs to be defined first
		pair<unordered_set<UnorderedPair<WorldObject*>>*, vector<WorldObject*>*> GetBroadCollisionsHelper(unordered_set<UnorderedPair<WorldObject*>>* store, qnode* node);
};