#pragma once

template <typename T>
class DeltaProvider
{
	public:
		virtual T Tick() = 0;
		~DeltaProvider();
};

template<typename T>
inline DeltaProvider<T>::~DeltaProvider()
{
}
