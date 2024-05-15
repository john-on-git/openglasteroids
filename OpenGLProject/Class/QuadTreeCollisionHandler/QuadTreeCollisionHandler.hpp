#pragma once

#include "../ICollisionHandler/ICollisionHandler.hpp"
#include <glm/ext/vector_float2.hpp>


class QuadTreeCollisionHandler : public ICollisionHandler {
	public:
		QuadTreeCollisionHandler(unsigned char maxDepth, glm::vec2 initialBounds[2]);
		virtual void Update(vector<WorldObject*> v);
		virtual unordered_set<UnorderedPair<WorldObject*>>* GetBroadCollisions();
		virtual bool GetFineCollision(WorldObject* a, WorldObject* b);
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

				qnode(qnode* parent, glm::vec2 bounds[2])
				{
					this->parent = parent;

					children[0] = NULL;
					children[1] = NULL;
					children[2] = NULL;
					children[3] = NULL;

					this->bounds[0] = bounds[0],
					this->bounds[1] = bounds[1];
				}

				void Clear()
				{
					for (auto child : children)
						if(child!=NULL)
							child->Clear();
					delete this;
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
							//try to fit it in a child first
							bool inSubTree = false; //true if the object has been inserted somewhere in the subtree
							if (children[0] == NULL) //if this is a leaf node, create the children
							{
								children[0] = new qnode( //top left
									this,
									new glm::vec2[]{
										glm::vec2( //top left
											this->bounds[0].x,
											this->bounds[0].y
										),
										glm::vec2( //bottom right
											(this->bounds[0].x + this->bounds[1].x) / 2,
											(this->bounds[0].y + this->bounds[1].y) / 2
										)
									}
								);
								children[1] = new qnode( //top right
									this,
									new glm::vec2[]{
										glm::vec2( //top left
											(this->bounds[0].x + this->bounds[1].x) / 2,
											this->bounds[0].y
										),
										glm::vec2( //bottom right
											this->bounds[1].x,
											(this->bounds[0].y + this->bounds[1].y) / 2
										)
									}
								);
								children[2] = new qnode( //bottom left
									this,
									new glm::vec2[]{
										glm::vec2( //top left
											this->bounds[0].x,
											(this->bounds[0].y + this->bounds[1].y) / 2
										),
										glm::vec2( //bottom right
											(this->bounds[0].x + this->bounds[1].x) / 2,
											this->bounds[1].y
										)
									}
								);
								children[3] = new qnode( //bottom right
									this,
									new glm::vec2[]{
										glm::vec2( //top left
											(this->bounds[0].x + this->bounds[1].x) / 2,
											(this->bounds[0].y + this->bounds[1].y) / 2
										),
										glm::vec2( //bottom right
											this->bounds[1].x,
											this->bounds[1].y
										)
									}
								);
							}
							for (int i = 0;i < 4;i++)
								inSubTree = inSubTree || children[i]->TryInsert(maxDepth, currentDepth + 1, obj);
							//if it did not fit anywhere in the subtree, put it in this node
							if (!inSubTree)
							{
								contents.push_back(obj);
								return true;
							}
						}
						return false; //failed to insert
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
			protected:
				bool WillFit(WorldObject* obj)
				{
					glm::vec3 absCoords[]{
						obj->model->bBox[0] + obj->position,
						obj->model->bBox[1] + obj->position
					};

					return	(bounds[0].x <= absCoords[0].x) && (bounds[0].y >= absCoords[0].y) && //top left
							(bounds[1].x >= absCoords[1].x) && (bounds[1].y <= absCoords[1].y); //bottom right
				}
		};
		qnode* root;
		pair<unordered_set<UnorderedPair<WorldObject*>>*, vector<WorldObject*>*> GetBroadCollisionsHelper(unordered_set<UnorderedPair<WorldObject*>>* store, qnode* node);
};