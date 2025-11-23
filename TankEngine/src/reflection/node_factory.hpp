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

			/// <summary>
			/// Map type from class name to constructor.
			/// </summary>
			typedef std::map<std::string, constructor_t> class_map_t;

		private:
			class_map_t m_classMap;


			/// <summary>
			/// (Optionally) Gets the constructor matching the name provided.
			/// </summary>
			/// <param name="name">The name of the constructor's class.</param>
			/// <returns>(Optional) The constructor.</returns>
			std::optional<constructor_t> tryGetClass(std::string const &name)
			{
				class_map_t::iterator it = m_classMap.find(name);
				if (it == m_classMap.end()) return {};
				return it->second;
			}
		public:
			/// <summary>
			/// An abstract zero-arg constructor.
			/// </summary>
			template <class T>
			static void *construct()
			{
				return new T();
			}

			/// <summary>
			/// Registers a class name, by mapping it to a personal constructor.
			/// </summary>
			/// <typeparam name="T">The type of the class.</typeparam>
			/// <param name="name">The name given to the class.</param>
			template <class T>
			void registerClass(std::string const &name)
			{
				constructor_t constructor = &NodeFactory::construct<T>;
				m_classMap.insert(std::make_pair(name, constructor));
			}


			/// <summary>
			/// Constructs the registered class matching the given name (or causes a critical error).
			/// Then deserialises it.
			/// </summary>
			/// <returns>The constructed object, or nullptr.</returns>
			Node *create(const json &serialised)
			{
				// Deduce type
				std::string type = serialised["type"];

				auto classType = tryGetClass(type);
				if (!classType.has_value())
				{
					TE_CORE_CRITICAL(std::format("Failed to construct {}: no class found", type));
					return nullptr;
				}

				constructor_t constructor = classType.value();

				Node *node = (Node *)constructor();
				node->deserialise(serialised);
				return node;
			}
		};
	}
}