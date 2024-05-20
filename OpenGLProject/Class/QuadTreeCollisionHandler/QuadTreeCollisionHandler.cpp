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

void QuadTreeCollisionHandler::Update(vector<WorldObject*> v)
{
	if(root!=NULL) //if there's an existing quadtree
		delete root; //clear it
	//construct the quadtree
	root = new qnode(NULL, initialBounds[0], initialBounds[1]);
	for (auto obj : v)
		root->TryInsert(maxDepth, 1, obj);
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
bool LineIntersectsPolygon(glm::vec4 e, WorldObject* q, glm::vec4 qPosition)
{
	//TODO, there's actually no class containing the vector positions as this info is stored on the GPU
	//so this data needs to be added to q/its components

	//based on Eric Haines - Fast Ray-Convex Polyhedron Intersection
	
	glm::vec2 intersection = glm::vec2(0.0f, glm::length(e)); //x = tnear, y = tfar
	//TODO the worldobject q should contain a representation of the faces
	//for each face of q
		//find the POI
		//branch? updating the tnear/tfar
	return false;
}

bool QuadTreeCollisionHandler::GetFineCollision(WorldObject* p, WorldObject* q)
{
	//based on the polygon intersection algorithm detailed by The Morgan Kaufmann Series in Interactive 3D Technology
	
	//if any of the edges of p intersect q
	//if any of the edges of q intersect p
	//special case for identical & aligned shapes
	return false;
}