#pragma once


template <class T> class UnorderedPair
{
	public:
		UnorderedPair()
		{
			this->a = NULL;
			this->b = NULL;
		}
		UnorderedPair(T a, T b)
		{
			this->a = a;
			this->b = b;
		}
		bool operator == (const UnorderedPair<T>& other) const
		{
			return (other.a == a && other.b == b) || (other.b == a && other.a == b);
		}
		bool operator < (const UnorderedPair<T>& other) const
		{
			return true;
		}
		T a;
		T b;
	private:
};

namespace std {
	template<class T> struct hash<UnorderedPair<T>>
	{
		size_t operator()(const UnorderedPair<T> & p) const
		{
			size_t hashA = hash<T>()(p.a);
			size_t hashB = hash<T>()(p.b);
			return hashA ^ hashB; //stack says not to do this but I don't care
		}
	};
}
