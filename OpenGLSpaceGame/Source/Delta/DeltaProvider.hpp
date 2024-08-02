#pragma once

template <typename T>
class DeltaProvider
{
	public:
		virtual T Tick() = 0;
};