#pragma once
#include "../WorldObject/WorldObject.hpp"
#include "../UnorderedPair/UnorderedPair.hpp"
#include <vector>
#include <unordered_set>
using namespace std;

class ICollisionHandler
{
	public:
		virtual void Update(vector<WorldObject*> v) = 0;
		virtual unordered_set<UnorderedPair<WorldObject*>>* GetBroadCollisions() = 0;
		virtual bool GetFineCollision(WorldObject* a, WorldObject* b) = 0;
};