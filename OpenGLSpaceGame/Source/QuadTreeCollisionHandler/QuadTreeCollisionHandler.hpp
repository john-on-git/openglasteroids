	#pragma once

#include <iostream>
#include "../ICollisionHandler/ICollisionHandler.hpp"
#include <glm/ext/vector_float2.hpp>


class QuadTreeCollisionHandler : public ICollisionHandler {
	public:
		QuadTreeCollisionHandler(unsigned char maxDepth, glm::vec2 bottomLeft, glm::vec2 topRight);
		virtual void Register(WorldObject* o);
		virtual void Remove(WorldObject* o);
		virtual unordered_set<UnorderedPair<WorldObject*>>* Check();
		virtual glm::vec2* GetNodeBoundsForObject(WorldObject* object);
		vector<glm::vec2*>* GetAllBounds(vector<glm::vec2*>* store);
	private:
		virtual unordered_set<UnorderedPair<WorldObject*>>* GetBroadCollisions();
		
		unordered_set<WorldObject*> objects;
		glm::vec2 initialBounds[2];
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

					children[0] = nullptr;
					children[1] = nullptr;
					children[2] = nullptr;
					children[3] = nullptr;

					bounds[0] = bottomLeft;
					bounds[1] = topRight;
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
							if (children[0] == nullptr) //if this is a leaf node, create the children
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
							for (char i = 0; i < 4 && !inSubTree;i++) {
								inSubTree = inSubTree || children[i]->TryInsert(maxDepth, currentDepth + 1, obj);
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
					if(children[0]!=nullptr) //if not a leaf, check children
						for (auto child : children)
						{
							auto res = child->Find(obj);
							if (res != nullptr)
								return res;
						}
					return nullptr; //not in tree
				}
				void DepthFirstFlatten(vector<glm::vec2*>* collector)
				{
					auto heapCopy = new glm::vec2[2]{ bounds[0], bounds[1] };
					collector->push_back(heapCopy);
					for (auto child : children)
					{
						if (child != nullptr) {
							child->DepthFirstFlatten(collector);
						}
					}
				}
			protected:
				bool WillFit(WorldObject* obj) const
				{
					//calculate world coordinates of object's object-aligned bounding box
					auto objBBox = obj->getOrientedBoundingBox();
					//determine whether the OABB is located fully inside this region
					for (char i=0;i<8;i++)
					{
						auto point = objBBox[i];
						auto result = point.x >= bounds[0].x && point.y >= bounds[0].y && //bottom left
							point.x <= bounds[1].x && point.y <= bounds[1].y; //top right
						if (!result) {
							return false;
						}
					}
					return true;
				}
		};
		qnode* root; //the root node of the quadtree. here instead of at the top because qnode needs to be defined first
		vector<WorldObject*>* GetBroadCollisionsHelper(unordered_set<UnorderedPair<WorldObject*>>* store, qnode* node);
};