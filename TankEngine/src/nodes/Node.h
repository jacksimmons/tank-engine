#pragma once
#include <ecs/Entity.h>
#include "interfaces/Serialisable.h"
#include "interfaces/Scriptable.h"


namespace Tank
{
	class Transform;
	class KeyInput;
	class Script;
	class Node;


	/// <summary>
	/// An object which exists in the Node hierarchy. It has a parent (or is the root),
	/// and any number of children.
	/// 
	/// Nodes can be serialised and deserialised.
	/// </summary>
	class TANK_API Node : public Entity, public ISerialisable<>, public IScriptable
	{
	protected:
		std::string m_type;
		std::vector<std::unique_ptr<Script>> m_scripts;
	public:
		Node(const std::string &name = "Node");
		virtual ~Node();


		void addScript(std::unique_ptr<Script>);

		/// @brief Removes a script.
		/// @param script 
		/// @return Whether it was successfully removed.
		bool removeScript(const Res &path);

		std::vector<Res> getScriptPaths();
	};
}