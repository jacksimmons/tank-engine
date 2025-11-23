#pragma once


namespace Tank
{
	namespace Reflect
	{
		namespace Constructor
		{
			/// <summary>
			/// An abstract constructor, with a required argument list.
			/// - Specify the constructor arguments with ...TArgs.
			/// </summary>
			template <class T, class ...TArgs>
			void *withArgs(TArgs&&... args)
			{
				return new T(std::forward<TArgs>(args));
			}
		}
	}
}