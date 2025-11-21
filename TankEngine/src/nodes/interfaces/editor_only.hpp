#pragma once
#include "scriptable.hpp"
#include "nodes/node.hpp"


namespace Tank
{
	/// <summary>
	/// A Node which becomes invisible on startup, and visible on shutdown.
	/// Applies the same behaviour to first-generation children.
	/// 
	/// @todo Toggling enable/disable of a parent may break this system.
	/// </summary>
	class TANK_API IEditorOnly : public Node
	{
	protected:
		IEditorOnly(const std::string &name);
	public:
		virtual ~IEditorOnly() = default;

		virtual void startup() override;
		virtual void shutdown() override;
	};
}