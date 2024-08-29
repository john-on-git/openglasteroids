#include "QuadTreeCollisionHandler.hpp"
#include "../Model/Model.hpp"
#include <iostream>
#include <queue>
#include <glm/geometric.hpp>

QuadTreeCollisionHandler::QuadTreeCollisionHandler(unsigned char maxDepth, glm::vec2 bottomLeft, glm::vec2 topRight)
{
	this->maxDepth = maxDepth;
	initialBounds[0] = bottomLeft;
	initialBounds[1] = topRight;
	root = NULL;
}

void QuadTreeCollisionHandler::Register(WorldObject* o)
{
	this->objects.insert(o);
}

unordered_set<UnorderedPair<WorldObject*>>* QuadTreeCollisionHandler::Check()
{
	unordered_set<UnorderedPair<WorldObject*>>* collisions = new unordered_set<UnorderedPair<WorldObject*>>();
	
	//clear any existing quadtree
	if (root != NULL)
	{
		delete root; //clear it
	}

	//and construct the new one
	root = new qnode(NULL, initialBounds[0], initialBounds[1]);
	for (auto obj : this->objects)
	{
		root->TryInsert(maxDepth, 1, obj);
	}

	//check collision
	auto broadCollisions = this->GetBroadCollisions();
	for (auto it = broadCollisions->begin(); it != broadCollisions->end(); it++) //for each collision
	{
		if (this->GetFineCollision(it->first, it->second))
		{
			collisions->insert(*it);
		}
	}
	return collisions;
}

unordered_set<UnorderedPair<WorldObject*>>* QuadTreeCollisionHandler::GetBroadCollisions()
{
	auto o = new unordered_set<UnorderedPair<WorldObject*>>;
	if (root == NULL) //this has never been updated
		return NULL;
	else
	{
		auto pair = GetBroadCollisionsHelper(o, root);
		delete pair.second;
		return pair.first;
	}
}

pair<unordered_set<UnorderedPair<WorldObject*>>*, vector<WorldObject*>*> QuadTreeCollisionHandler::GetBroadCollisionsHelper(unordered_set<UnorderedPair<WorldObject*>>* store, qnode* node)
{
	//methods?
		//iterating the tree a bunch
		//caching subtree contents in qnode struct
		//returning cache from function & taking union (doing this one)
	auto cache = new vector<WorldObject*>;

	if (node->children[0] != NULL) //if this is not a leaf node, check subtree first
		for (char i = 0;i < 4;i++)
		{
			//get all content from subtree & move to this node's cache
			vector<WorldObject*>* subCache = GetBroadCollisionsHelper(store, node->children[i]).second;
			cache->insert(cache->end(),
				std::make_move_iterator(subCache->begin()),
				std::make_move_iterator(subCache->end())
			);
			delete subCache;
		}

	//check this node
	for (auto item : node->contents)
	{
		for (auto other : *cache)
			if(item!=other)
				store->emplace(UnorderedPair<WorldObject*>(item, other));
		cache->push_back(item);
	}
	
	return pair<unordered_set<UnorderedPair<WorldObject*>>*, vector<WorldObject*>*>(store, cache);
}

//for debugging
glm::vec2* QuadTreeCollisionHandler::GetNodeBoundsForObject(WorldObject* target)
{
	//bfs the tree for object
	qnode* result = root->Find(target);
	return result==NULL ? NULL : result->bounds;
}

vector<glm::vec2*>* QuadTreeCollisionHandler::GetAllBounds(vector<glm::vec2*>* store)
{
	root->DepthFirstFlatten(store);
	return store;
}

/**
* 
* @param e vector representing the line segment, starting at the origin
* @param q the model to check for intersection with
* @param qPosition the position of q relative to the start of e
* @return true if the line intersects the model, else false
*/
static bool LineIntersectsPolygon(glm::mat2x4 edge, vector<glm::vec4>* faces)
{
	//TODO, there's actually no class containing the vector positions as this info is stored on the GPU
	//so this data needs to be added to q/its components
	//Done!

	//based on Eric Haines - Fast Ray-Convex Polyhedron Intersection
	
	double tNear = 0;
	double tFar = glm::length(glm::vec3(edge[1] - edge[0])); //which is glm::length(rayDirection);

	auto rayOrigin = glm::vec3(edge[0]);
	auto rayDirection = glm::normalize(glm::vec3(edge[1] - edge[0]));

	for (unsigned int i = 0;i < faces->size();i++) //for each face of q 
	{
		glm::vec4 pn = faces->at(i);	
		auto vd = glm::dot(glm::vec3(pn), rayDirection);
		auto vn = glm::dot(glm::vec3(pn), rayOrigin) - pn.w;
		if (vd == 0)
		{
			//"if vd is 0 then the ray is parallel and no intersection takes place"
			//"in such a case, we check if the ray origin is inside the plane's half space"

			if (vn > 0) //"if vn is positive... the ray must miss the polyhedron, so testing is done."
			{
				return false;
			}
		}
		else
		{
			auto t = -vn / vd;
			if (vd > 0) //"if vd is positive the plane is back-facing"
			{
				//"t can affect tfar, if t<0, then the polygon is missed."
				if (t < 0)
				{
					return false;
				}
				else if (t < tFar) //"if t is less than tfar, tfar is set to t"
				{
					tFar = t;
				}
			}
			else //"if vd is negative, it is front-facing."
			{
				if (t > tNear)
				{
					tNear = t;
				}
			}

			//"if tNear ever is greater than tFar, the ray must miss the polyhedron"
			if (tNear > tFar)
			{
				return false;
			}
		}
	}
	return true;
}
bool QuadTreeCollisionHandler::GetFineCollision(WorldObject* p, WorldObject* q)
{
	//based on the polygon intersection algorithm detailed by The Morgan Kaufmann Series in Interactive 3D Technology

	//TODO for debugging, remove me!
	/*std::cout << "Didn't Collide w/ Faces:" << endl;
	std::cout << "p:" << endl;
	auto pFaces = p->calcFaces();
	for (unsigned int i = 0; i < pFaces->size(); i++)
	{
		auto face = pFaces->at(i);
		std::cout << "\t" << "(" << face.x << "x + " << face.y << "y + " << face.z << "z) =" << face.w << endl;
	}
	std::cout << "q:" << endl;

	auto qFaces = q->calcFaces();
	for (unsigned int i = 0; i < qFaces->size(); i++)
	{
		auto face = qFaces->at(i);
		std::cout << "\t" << "(" << face.x << "x + " << face.y << "y + " << face.z << "z) =" << face.w << endl;
	}*/

	//one issue is that the edges aren't scaled. Fixed!

	//README! Currently trying to go case-by case on the first call. edge 18 of p intersects (but not really). rayDirection is NaN for it.
	std::vector<glm::mat2x4>* edges;
	std::vector<glm::vec4>* faces;

	//the objects are intersecting if any of the edges of p intersect q
	edges = p->calcEdges();
	faces = q->calcFaces();
	for (unsigned int i = 0;i < edges->size();i++)
	{
		//return true if this edge intersects the polygon, transforming everything so the start of the edge is the origin
		if (LineIntersectsPolygon(edges->at(i), faces)) //TODO
		{
			return true;
		}
	}
	delete faces;
	delete edges;

	//or if any of the edges of q intersect p
	edges = q->calcEdges();
	faces = p->calcFaces();
	for (unsigned int i = 0;i < edges->size();i++)
	{
		//return true if this edge intersects the polygon, transforming everything so the start of the edge is the origin
		if (LineIntersectsPolygon(edges->at(i), faces)) //TODO
		{
			return true;
		}
	}
	delete faces;
	delete edges;

	//TODO special case for identical & aligned shapes

	return false; //no intersection found
}