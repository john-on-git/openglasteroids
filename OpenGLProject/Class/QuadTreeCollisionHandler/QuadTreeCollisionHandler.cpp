#include "QuadTreeCollisionHandler.hpp"
#include <iostream>
#include <queue>

QuadTreeCollisionHandler::QuadTreeCollisionHandler(unsigned char maxDepth, glm::vec2* initialBounds)
{
	this->maxDepth = maxDepth;
	this->initialBounds = initialBounds;
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

bool QuadTreeCollisionHandler::GetFineCollision(WorldObject* a, WorldObject* b)
{
	//TODO
	//https://web.archive.org/web/20141127210836/http://content.gpwiki.org/index.php/Polygon_Collision
	glm::vec2 temp;
	//get vector between closest points

	//temp = temp; //get a vector perpendicular to it

	//get the shadows of a & b on vector

	return false;/*do the shadows overlap*/
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

vector<glm::vec2*>* QuadTreeCollisionHandler::GetAllBounds() 
{
	vector<glm::vec2*>* vector = new std::vector<glm::vec2*>;
	root->DepthFirstFlatten(vector);
	return vector;
}