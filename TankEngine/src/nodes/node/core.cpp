#include <nodes/Node.h>
#include <KeyInput.h>
#include <Transform.h>
#include <scripting/Script.h>


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