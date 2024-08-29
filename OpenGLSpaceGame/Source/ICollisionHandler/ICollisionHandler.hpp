#pragma once
#include "../WorldObject/WorldObject.hpp"
#include "../UnorderedPair/UnorderedPair.hpp"
#include <vector>
#include <unordered_set>
using namespace std;

class ICollisionHandler
{
	public:
		virtual void Register(WorldObject* o) = 0;
		virtual unordered_set<UnorderedPair<WorldObject*>>* Check() = 0;
};