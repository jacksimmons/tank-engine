#pragma once


namespace Tank
{
	namespace Utils
	{
		template <class A, class B>
		static std::pair<B, A> flipPair(const std::pair<A, B> &pair)
		{
			return std::pair<B, A>(pair.second, pair.first);
		}


		template <class A, class B, template<class, class, class...> class M, class ...Args>
		static std::multimap<B, A> flipMap(const M<A, B, Args...> &src)
		{	
			std::multimap<B, A> dest;
			std::transform(src.begin(), src.end(), std::inserter(dest, dest.begin()), flipPair<A, B>);
			return dest;
		}
	}
}