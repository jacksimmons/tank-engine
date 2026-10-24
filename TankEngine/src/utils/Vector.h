#pragma once
#include <memory>


namespace Tank
{
	namespace Vector
	{
		namespace UniquePtr
		{
			/// @brief Converts std::vector<std::unique_ptr<T>> to std::vector<T*> without moving.
			template <class T>
			static std::vector<T *> toRawPtr(const std::vector<std::unique_ptr<T>> &uniquePtrs)
			{
				std::vector<T *> rawPtrs;
				for (const std::unique_ptr<T> &uniquePtr : uniquePtrs)
				{
					rawPtrs.push_back(uniquePtr.get());
				}
				return rawPtrs;
			}
		}
	}
};