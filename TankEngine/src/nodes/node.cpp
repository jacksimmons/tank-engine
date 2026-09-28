#include <GLFW/glfw3.h>
#include <scripting/Script.h>
#include <Log.h>
#include <KeyInput.h>
#include <events/EventManager.h>
#include <components/Transform.h>
#include "Node.h"


namespace Tank
{


	void Node::addScript(std::unique_ptr<Script> script)
	{
		if (!m_keyInput)
		{
			// Create Editor KeyInput
			std::vector<int> registeredKeys = {
				// Function keys
				GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5, GLFW_KEY_F6,
				// Cam Movement keys
				GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E,
				// Cam Rotation keys
				GLFW_KEY_I, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_U, GLFW_KEY_O,
			};
			m_keyInput = std::make_unique<KeyInput>(registeredKeys);
		}

		m_scripts.push_back(std::move(script));
	}


	bool Node::removeScript(const Res &path)
	{
		auto it = std::find_if(m_scripts.begin(), m_scripts.end(), [&path](std::unique_ptr<Script> &ownedScript)
		{
			if (ownedScript.get()->getPath() == path)
			{
				return true;
			}
			return false;
		});

		if (it == m_scripts.end()) return false;
		m_scripts.erase(it);
		return true;
	}


	std::vector<Res> Node::getScriptPaths()
	{
		std::vector<Res> scriptPaths;
		for (const auto &script : m_scripts)
		{
			scriptPaths.push_back(script->getPath());
		}
		return scriptPaths;
	}
}


json Tank::Node::serialise()
{
	json serialised;
	serialised["name"] = m_name;
	serialised["type"] = m_type;
	serialised["enabled"] = m_enabled;
	serialised["visible"] = m_visible;
	serialised["transform"] = TransformComponent::serialise(&getComponent<TransformComponent>());

	std::vector<std::string> scriptNames;
	for (const auto &script : m_scripts)
	{
		scriptNames.push_back(Res::encode(script->getPath()));
	}
	serialised["scripts"] = scriptNames;

	return serialised;
}

void Tank::Node::deserialise(const json &serialised)
{
	setName(serialised["name"]);
	m_enabled = serialised["enabled"];
	m_visible = serialised["visible"];
	TransformComponent::deserialise(serialised["transform"], &getComponent<TransformComponent>());

	for (const auto &script : serialised["scripts"].get<std::vector<std::string>>())
	{
		addScript(Script::createScript(this, Res::decode(script)).value());
	}
}
