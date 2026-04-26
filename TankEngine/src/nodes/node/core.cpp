#include <nodes/node.hpp>
#include <key_input.hpp>
#include <transform.hpp>
#include <scripting/script.hpp>


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