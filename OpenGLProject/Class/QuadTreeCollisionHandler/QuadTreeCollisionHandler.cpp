#include "QuadTreeCollisionHandler.hpp"
#include <iostream>

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
	for (auto obj : v)
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
	if(node->children[0]!=NULL) //if this is not a leaf node, check subtree first
		for (char i = 0;i < 4;i++)
			GetBroadCollisionsHelper(store, node->children[i]);

	for (auto item : node->contents) //check this node
		for (auto other : *node->UnionOfSubTreeContentsExcludingtarget(item))
			store->emplace(UnorderedPair<WorldObject*>(item, other));
	return store;
}