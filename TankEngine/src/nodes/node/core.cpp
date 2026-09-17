#include <nodes/Node.hpp>
#include <KeyInput.hpp>
#include <Transform.hpp>
#include <scripting/Script.hpp>


namespace Tank
{
	Node::Node(const std::string &name)
	{
		m_type = "Node";
		m_name = name;
		m_transform = std::make_unique<Transform>(this);
		m_keyInput = std::unique_ptr<KeyInput>(nullptr);
	}


	Node::~Node() = default;
}