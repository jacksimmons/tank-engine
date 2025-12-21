#include <log.hpp>
#include "string.hpp"


namespace Tank::String
{
	void replaceAll(std::string &str, const std::string &exp, size_t maxCount)
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
}