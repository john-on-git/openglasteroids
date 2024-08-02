#pragma once

template <typename T>
class DeltaTarget
{
	public:
		virtual void Tick(T change, float maximumMagnitude) = 0;
};