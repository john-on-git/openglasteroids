#pragma once

#include <vector>
#include "DeltaProvider.hpp"
#include "DeltaTarget.hpp"

template <typename T>
class Delta
{
public:
	long long duration;
	Delta(DeltaProvider<T>* provider);
	Delta(DeltaProvider<T>* provider, long long duration);
	Delta(DeltaProvider<T>* provider, long long duration, float maximumMagnitude);
	~Delta();
	void AddTarget(DeltaTarget<T>* target);
	bool Tick();
protected:
	float maximumMagnitude;
	std::vector<DeltaTarget<T>*> targets;
	DeltaProvider<T>* provider;
};

//stackoverflow suggests that the linker errors are caused by putting this in its own .cpp file.

template<typename T>
Delta<T>::Delta(DeltaProvider<T>* provider)
{
	this->provider = provider;
	this->duration = -1;
	this->maximumMagnitude = -1;
}
template<typename T>
Delta<T>::Delta(DeltaProvider<T>* provider, long long duration)
{
	this->provider = provider;
	this->duration = duration;
	this->maximumMagnitude = -1;
}
template<typename T>
Delta<T>::Delta(DeltaProvider<T>* provider, long long duration, float maximumMagnitude)
{
	this->provider = provider;
	this->duration = duration;
	this->maximumMagnitude = maximumMagnitude;
}

template<typename T>
Delta<T>::~Delta()
{
	this->targets.clear();
	delete this;
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
		for (unsigned int i = 0;i < this->targets.size();i++) //apply the change to each target
		{
			this->targets.at(i)->Tick(change, this->maximumMagnitude);
		}
	}
	if (duration > 0) //update the duration
		duration--;
	return duration == 0;
}

template<typename T>
void Delta<T>::AddTarget(DeltaTarget<T>* target)
{
	this->targets.push_back(target);
}