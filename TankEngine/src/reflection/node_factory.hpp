#pragma once
#include <log.hpp>
#include "constructor.hpp"


namespace Tank
{
	class Node;
	namespace Reflect
	{
		class TANK_API NodeFactory
		{
			typedef void *(*constructor_t)();

			typedef std::map<std::string, constructor_t> name_to_class_t;

		private:
			name_to_class_t m_nameToClassMap;


			/// <summary>
			/// An abstract zero-arg constructor.
			/// </summary>
			template <class T>
			static void *construct()
			{
				return new T();
			}

			/// <summary>
			/// (Optionally) Gets the constructor matching the name provided.
			/// </summary>
			/// <param name="name">The name of the constructor's class.</param>
			/// <returns>(Optional) The constructor.</returns>
			std::optional<constructor_t> tryGetClass(std::string const &name)
			{
				name_to_class_t::iterator it = m_nameToClassMap.find(name);
				if (it == m_nameToClassMap.end()) return {};
				return it->second;
			}
		public:
			/// <summary>
			/// Registers a name-class pair.
			/// </summary>
			/// <typeparam name="T">The type of the class.</typeparam>
			/// <param name="name">The name given to the class.</param>
			template <class T>
			void registerClass(std::string const &name)
			{
				constructor_t constructor = &NodeFactory::construct<T>;
				m_nameToClassMap.insert(std::make_pair(name, constructor));
			}


			// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
			//           Deserialisation
			// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
			/// <summary>
			/// Constructs the registered class matching the given name (or causes a critical error).
			/// Then deserialises it.
			/// </summary>
			/// <returns>The constructed object, or nullptr.</returns>
			Node *deserialise(const json &serialised);
		};
	}
}