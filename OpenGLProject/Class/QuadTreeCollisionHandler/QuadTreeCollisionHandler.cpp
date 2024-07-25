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
bool LineIntersectsPolygon(glm::vec4 e, vector<glm::vec4>* faces)
{
	//TODO, there's actually no class containing the vector positions as this info is stored on the GPU
	//so this data needs to be added to q/its components
	//Done!

	//based on Eric Haines - Fast Ray-Convex Polyhedron Intersection
	
	glm::vec2 intersection = glm::vec2(0.0f, glm::length(e)); //x = tnear, y = tfar
	double tNear = 0;
	double tFar = std::numeric_limits<double>::max();
	//TODO the worldobject q should contain a representation of the faces
	for (unsigned int i = 0;i < faces->size();i++) //for each face of q
	{
		//find the POI
		glm::vec4 pn = faces->at(i);
		auto vd = glm::dot(pn, e); //calculate Vd
		auto vn = pn.w; //ray origin is always *the* origin, so vn = d;
		auto t = -vn/ vd;
		if (vd == 0)
		{
			//"if vd is 0 then the ray is parallel and no intersection takes place"
			//"in such a case, we check if the ray origin is inside the plane's half space"


			if (vn > 0) //"if vn is positive... the ray must miss the polygon, so testing is done."
			{
				return false;
			}
		}
		else if (vd > 0)//"if vd is positive the plane is back-facing"
		{
			//t can affect tfar, if t<0, then the polygon is missed. if t<tfar, tfar is set to t
			if (t < 0)
			{
				return false;
			}
			else if (t<tFar)
			{
				tFar = t;
				if (tNear > tFar)
				{
					return false;
				}
			}
		}
		else //"if vd is negative, it is front-facing."
		{
			if (t < 0)
			{
				return false;
			}
			else if (t > tNear)
			{
				tNear = t;
				if (tNear > tFar)
				{
					return false;
				}
			}
		}
		//branch? updating the tnear/tfar
	}
	return true;
}
bool QuadTreeCollisionHandler::GetFineCollision(WorldObject* p, WorldObject* q)
{
	//based on the polygon intersection algorithm detailed by The Morgan Kaufmann Series in Interactive 3D Technology
	
	//the objects are intersecting if any of the edges of p intersect q
	for (unsigned int i = 0;i < p->model->edges.size();i++)
	{
		glm::mat2x4 edge = p->model->edges.at(i);
		//return true if this edge intersects the polygon, transforming everything so the start of the edge is the origin
		std::vector<glm::vec4>* faces = q->calcFaces(glm::vec4(q->getPosition(), 0) - edge[0], q->getAngle());
		if (LineIntersectsPolygon(edge[1] - edge[0], faces)) //TODO
		{
			delete faces;
			return true;
		}
		delete faces;
	}
	//or if any of the edges of q intersect p
	for (unsigned int i = 0;i < q->model->edges.size();i++)
	{
		glm::mat2x4 edge = q->model->edges.at(i);
		//return true if this edge intersects the polygon, transforming everything so the start of the edge is the origin
		std::vector<glm::vec4>* faces = p->calcFaces(glm::vec4(p->getPosition(), 0) - edge[0], p->getAngle());
		if (LineIntersectsPolygon(edge[1] - edge[0], faces)) //TODO
		{
			delete faces;
			return true;
		}
		delete faces;
	}
	//TODO special case for identical & aligned shapes
	return false; //no intersection found
}