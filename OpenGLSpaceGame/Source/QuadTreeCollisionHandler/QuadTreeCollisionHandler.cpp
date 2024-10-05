#include "QuadTreeCollisionHandler.hpp"
#include "../Model/Model.hpp"
#include <iostream>
#include <queue>
#include <glm/geometric.hpp>
/**
*
* @param e vector representing the line segment, starting at the origin
* @param q the model to check for intersection with
* @param qPosition the position of q relative to the start of e
* @return true if the line intersects the model, else false
*/
static bool LineIntersectsPolygon(glm::mat2x4 edge, glm::vec4* faces, size_t nFaces)
{
	//TODO, there's actually no class containing the vector positions as this info is stored on the GPU
	//so this data needs to be added to q/its components
	//Done!

	//based on Eric Haines - Fast Ray-Convex Polyhedron Intersection

	auto dif = glm::vec3(edge[1] - edge[0]);

	double tNear = 0;
	double tFar = glm::length(dif); //which is glm::length(rayDirection);

	auto rayOrigin = glm::vec3(edge[0]);


	//auto rayDirection = glm::normalize(glm::vec3(edge[1] - edge[0]));
	//actually, manually normalize the vector to re-use the expensive magnitude calculation we just performed
	auto rayDirection = glm::vec3(dif.x/tFar, dif.y/tFar, dif.z/tFar);

	for (size_t i = 0; i < nFaces; i++) //for each face of q 
	{
		glm::vec4 pn = faces[i];
		auto vd = glm::dot(glm::vec3(pn), rayDirection);
		auto vn = glm::dot(glm::vec3(pn), rayOrigin) + pn.w;
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

static bool GetFineCollision(WorldObject* p, WorldObject* q)
{
	//based on the polygon intersection algorithm detailed by The Morgan Kaufmann Series in Interactive 3D Technology

	//one issue is that the edges aren't scaled. Fixed!

	//README! Currently trying to go case-by case on the first call. edge 18 of p intersects (but not really). rayDirection is NaN for it.
	glm::mat2x4* edges;
	glm::vec4* faces;

	//the objects are intersecting if any of the edges of p intersect q
	edges = p->calcEdges();
	faces = q->calcFaces();
	for (size_t i = 0; i < p->model->nEdges; i++)
	{
		//return true if this edge intersects the polygon, transforming everything so the start of the edge is the origin
		if (LineIntersectsPolygon(edges[i], faces, q->model->nFaces)) //TODO
		{
			return true;
		}
	}
	delete[] edges;
	delete[] faces;

	//or if any of the edges of q intersect p
	edges = q->calcEdges();
	faces = p->calcFaces();
	for (size_t i = 0; i < q->model->nEdges; i++)
	{
		//return true if this edge intersects the polygon, transforming everything so the start of the edge is the origin
		if (LineIntersectsPolygon(edges[i], faces, p->model->nFaces)) //TODO
		{
			return true;
		}
	}
	delete[] edges;
	delete[] faces;

	//TODO special case for identical & aligned shapes

	return false; //no intersection found
}

QuadTreeCollisionHandler::QuadTreeCollisionHandler(unsigned char maxDepth, glm::vec2 bottomLeft, glm::vec2 topRight)
{
	this->root = nullptr;
	this->maxDepth = maxDepth;
	initialBounds[0] = bottomLeft;
	initialBounds[1] = topRight;
}

void QuadTreeCollisionHandler::Register(WorldObject* o)
{
	this->objects.insert(o);
}

void QuadTreeCollisionHandler::Remove(WorldObject* o)
{
	this->objects.erase(o);
}

unordered_set<UnorderedPair<WorldObject*>>* QuadTreeCollisionHandler::Check()
{
	unordered_set<UnorderedPair<WorldObject*>>* collisions = new unordered_set<UnorderedPair<WorldObject*>>();

	//construct the quadtree
	root = new qnode(nullptr, initialBounds[0], initialBounds[1]);
	for (auto obj : this->objects)
	{
		root->TryInsert(maxDepth, 1, obj);
	}

	//check collision
	auto broadCollisions = this->GetBroadCollisions();
	for (auto it = broadCollisions->begin(); it != broadCollisions->end(); it++) //for each collision
	{
		auto firstBoundingSphereRadius = glm::length(glm::vec3(it->first->model->boundingMax) * it->first->getScale());
		auto secondBoundingSphereRadius = glm::length(glm::vec3(it->second->model->boundingMax) * it->second->getScale());
		auto spheresColliding = glm::length(it->first->getPosition() - it->second->getPosition()) <= firstBoundingSphereRadius + secondBoundingSphereRadius;

		if (spheresColliding && GetFineCollision(it->first, it->second))
		{
			collisions->insert(*it);
		}
	}
	delete root; //delete the quadtree
	delete broadCollisions;
	return collisions;
}

unordered_set<UnorderedPair<WorldObject*>>* QuadTreeCollisionHandler::GetBroadCollisions()
{
	auto o = new unordered_set<UnorderedPair<WorldObject*>>();
	vector<WorldObject*>* cache = GetBroadCollisionsHelper(o, root);
	delete cache;
	return o;
}

vector<WorldObject*>* QuadTreeCollisionHandler::GetBroadCollisionsHelper(unordered_set<UnorderedPair<WorldObject*>>* store, qnode* node)
{
	//methods?
		//iterating the tree a bunch
		//caching subtree contents in qnode struct
		//returning cache from function & taking union (doing this one)
	auto cache = new vector<WorldObject*>;

	if (node->children[0] != nullptr) //if this is not a leaf node, check subtree first
		for (char i = 0;i < 4;i++)
		{
			//get all content from subtree & move to this node's cache
			vector<WorldObject*>* subCache = GetBroadCollisionsHelper(store, node->children[i]);
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
	
	return cache;
}

//for debugging
glm::vec2* QuadTreeCollisionHandler::GetNodeBoundsForObject(WorldObject* target)
{
	//bfs the tree for object
	qnode* result = root->Find(target);
	return result==nullptr ? nullptr : result->bounds;
}

vector<glm::vec2*>* QuadTreeCollisionHandler::GetAllBounds(vector<glm::vec2*>* store)
{
	root->DepthFirstFlatten(store);
	return store;
}