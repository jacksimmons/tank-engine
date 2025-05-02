#pragma once
#include "node.hpp"


namespace Tank::Editor
{
	/// <summary>
	/// Disables itself on startup, and re-enables itself
	/// on shutdown = Only updates when not in play mode.
	/// </summary>
	class TANK_API EditorNode : public Node
	{
	public:
		EditorNode(const std::string &name);
		virtual ~EditorNode() = default;
		
		virtual void startup() override;
		virtual void shutdown() override;
	};
}