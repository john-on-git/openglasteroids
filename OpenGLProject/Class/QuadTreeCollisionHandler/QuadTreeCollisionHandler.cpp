#include "QuadTreeCollisionHandler.hpp"

QuadTreeCollisionHandler::QuadTreeCollisionHandler(unsigned char maxDepth, glm::vec2 initialBounds[2])
{
	this->maxDepth = maxDepth;
	this->initialBounds = initialBounds;
	this->root = NULL;
}

void QuadTreeCollisionHandler::Update(vector<WorldObject*> v)
{
	if(root!=NULL) //if there's an existing quadtree
		root->Clear(); //clear it
	//construct the quadtree
	root = new qnode(NULL, initialBounds);
	for(auto obj : v)
		root->TryInsert(maxDepth, 1, obj);
}

bool QuadTreeCollisionHandler::GetFineCollision(WorldObject* a, WorldObject* b)
{
	//TODO
	return false;
}

unordered_set<UnorderedPair<WorldObject*>>* QuadTreeCollisionHandler::GetBroadCollisions()
{
	auto o = new unordered_set<UnorderedPair<WorldObject*>>;
	if (root == NULL) //this has never been updated
		return NULL;
	else
		return GetBroadCollisionsHelper(o, root);
}

unordered_set<UnorderedPair<WorldObject*>>* QuadTreeCollisionHandler::GetBroadCollisionsHelper(unordered_set<UnorderedPair<WorldObject*>>* store, qnode* node)
{
	//shit code
	//TODO replace pair with non-directional pair if that makes any sense
	for (auto item : node->contents)
		for (auto other : *node->UnionOfSubTreeContentsExcludingtarget(item))
			store->emplace(UnorderedPair<WorldObject*>(item, other));
	return store;
}