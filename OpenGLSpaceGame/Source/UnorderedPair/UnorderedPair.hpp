#pragma once


template <class T> class UnorderedPair
{
	public:
		UnorderedPair()
		{
			this->first = NULL;
			this->second = NULL;
		}
		UnorderedPair(T a, T b)
		{
			this->first = a;
			this->second = b;
		}
		bool operator == (const UnorderedPair<T>& other) const
		{
			return (other.first == first && other.second == second) || (other.second == first && other.first == second);
		}
		bool operator < (const UnorderedPair<T>& other) const
		{
			return true;
		}
		T first;
		T second;
	private:
};

namespace std {
	template<class T> struct hash<UnorderedPair<T>>
	{
		size_t operator()(const UnorderedPair<T> & p) const
		{
			size_t hashA = hash<T>()(p.first);
			size_t hashB = hash<T>()(p.second);
			return hashA ^ hashB; //stack says not to do this but I don't care
		}
	};
}
