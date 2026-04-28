#include <log.hpp>
#include "string.hpp"


namespace Tank::String
{
	void eraseAll(std::string &str, const std::string &exp, size_t maxCount)
	{
		size_t count = 0;
		while (str.contains(exp) || count > maxCount)
		{
			size_t index = str.find(exp);
			str.erase(index, exp.size());
			count++;
		}

		if (count > maxCount)
		{
			TE_CORE_ERROR(std::format("String > replaceAll occurences exceeded max of {}.", maxCount));
		}
	}


	std::string toUpper(const std::string &original)
	{
		std::string copy;
		for (auto &c : original) copy += toupper(c);
		return copy;
	}
}