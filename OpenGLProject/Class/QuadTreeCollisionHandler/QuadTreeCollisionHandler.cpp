#include "QuadTreeCollisionHandler.hpp"
#include <iostream>
#include <queue>

QuadTreeCollisionHandler::QuadTreeCollisionHandler(unsigned char maxDepth, glm::vec2 initialBounds[2])
{
	this->maxDepth = maxDepth;
	this->initialBounds = initialBounds;
	this->root = NULL;
}

void QuadTreeCollisionHandler::Update(vector<WorldObject*> v)
{
	if(root!=NULL) //if there's an existing quadtree
		delete root; //clear it
	//construct the quadtree
	root = new qnode(NULL, initialBounds);
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
	//dfs the tree for object
	std::queue<qnode*> queue;
	queue.push(root);
	qnode* current = NULL;
	while (!queue.empty())
	{
		current = queue.front();
		queue.pop();
		for (auto worldObject = current->contents.begin(); worldObject != current->contents.end();worldObject++) {
			//if this is the target object is, return the current node's bounds
			if ((*worldObject) == target) {
				break;
			}
			else { //otherwise add all child nodes to queue
				for (int i = 0;i < 4;i++) {
					queue.push(current->children[i]);
				}
			}
		}
	}
	return current==NULL ? NULL : current->bounds;
}