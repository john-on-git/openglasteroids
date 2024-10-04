#pragma once

#include <vector>
#include "DeltaProvider.hpp"
#include "DeltaTarget.hpp"

using namespace std;

//this is all in the .hpp because it's a template

template <typename T>
class Delta
{
	public:
		long long duration;
		Delta(DeltaProvider<T>* provider, vector<DeltaTarget<T>*> targets, vector<Delta<T>*> dependents);
		Delta(DeltaProvider<T>* provider, vector<DeltaTarget<T>*> targets, vector<Delta<T>*> dependents, long long duration);
		Delta(DeltaProvider<T>* provider, vector<DeltaTarget<T>*> targets, vector<Delta<T>*> dependents, long long duration, float maximumMagnitude);
		~Delta();
		bool Tick();
		void addDependent(Delta<T>* dependent);

	protected:
		float maximumMagnitude;
		DeltaProvider<T>* provider;
		vector<DeltaTarget<T>*> targets;
		vector<Delta<T>*> dependents;
};

//stackoverflow suggests that the linker errors are caused by putting method definitions in a .cpp file.

template<typename T>
Delta<T>::Delta(DeltaProvider<T>* provider, vector<DeltaTarget<T>*> targets, vector<Delta<T>*> dependents)
{
	this->provider = provider;
	this->targets = targets;
	this->dependents = dependents;
	this->duration = -1;
	this->maximumMagnitude = -1;
}
	
template<typename T>
Delta<T>::Delta(DeltaProvider<T>* provider, vector<DeltaTarget<T>*> targets, vector<Delta<T>*> dependents, long long duration)
{
	this->provider = provider;
	this->targets = targets;
	this->dependents = dependents;
	this->duration = duration;
	this->maximumMagnitude = -1;
}
template<typename T>
Delta<T>::Delta(DeltaProvider<T>* provider, vector<DeltaTarget<T>*> targets, vector<Delta<T>*> dependents, long long duration, float maximumMagnitude)
{
	this->provider = provider;
	this->targets = targets;
	this->dependents = dependents;
	this->duration = duration;
	this->maximumMagnitude = maximumMagnitude;
}

template<typename T>
Delta<T>::~Delta()
{
	delete provider;
	for (auto target : targets)
	{
		delete target;
	}
	targets.clear();
	for (auto delta : dependents)
	{
		delete delta;
	}
	dependents.clear();
}

template <typename T>
/// <summary>
/// apply the delta to all targets, once
/// </summary>
/// <returns>true if this is finished</returns>
bool Delta<T>::Tick()
{
	if (duration != 0) { //not disjoint with the next condition, because negative means infinite duration
		T change = this->provider->Tick(); //get the change
		for (size_t i = 0;i < this->targets.size();i++) //apply the change to each target
		{
			this->targets.at(i)->Tick(change, this->maximumMagnitude);
		}
	}
	if (duration > 0) //update the duration
		duration--;
	//apply all dependent deltas, deleting any that have expired
	vector<Delta*> toDelete = vector<Delta*>();
	for (size_t i = 0; i < dependents.size(); i++)
	{
		if (dependents.at(i)->Tick()) //returns true when expired
		{
			delete dependents.at(i);
			dependents.erase(dependents.begin() + i);
			i--;
		}
	}
	return duration == 0;
}

template<typename T>
void Delta<T>::addDependent(Delta<T>* dependent)
{
	dependents.push_back(dependent);
}