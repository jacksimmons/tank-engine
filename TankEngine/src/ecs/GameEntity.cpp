#include <GLFW/glfw3.h>
#include <KeyInput.h>
#include <components/Tree.h>
#include <scripting/Script.h>
#include "GameEntity.h"
#include "Entity.h"


namespace Tank
{
	KeyInput *GameEntity::keyInput() const { return m_keyInput.get(); }


	void GameEntity::startup()
	{
		if (!isEnabled()) return;
		if (m_started) return;
		m_started = true;

		for (auto const &child : tree().m_children)
		{
			child->startup();
		}
	}


	void GameEntity::shutdown()
	{
		if (!isEnabled()) return;
		if (!m_started) return;
		m_started = false;

		for (auto const &child : tree().m_children)
		{
			child->shutdown();
		}
	}


	void GameEntity::addScript(std::unique_ptr<Script> script)
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


	bool GameEntity::removeScript(const Res &path)
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


	std::vector<Res> GameEntity::getScriptPaths()
	{
		std::vector<Res> scriptPaths;
		for (const auto &script : m_scripts)
		{
			scriptPaths.push_back(script->getPath());
		}
		return scriptPaths;
	}


	void GameEntity::update()
	{
		if (!isEnabled()) return;
		if (isVisible()) draw();

		preupdate();

		// Handle scripts
		if (m_started)
		{
			for (const auto &script : m_scripts)
			{
				if (script->getEnabled())
				{
					script->update();
				}
			}
		}

		// Update KeyInput (decay inputs)
		if (m_keyInput) m_keyInput->update();

		// Recursively update
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			(*it)->update();
		}
	}


	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise(GameEntity *in)
	{
		json serialised = serialise<Entity>(in);
		std::vector<std::string> scriptNames;
		for (const auto &scriptRes : in->getScriptPaths())
		{
			scriptNames.push_back(Res::encode(scriptRes));
		}
		serialised["scripts"] = scriptNames;
	}

	template<>
	void deserialise(const json &serialised, GameEntity *out)
	{
		deserialise<Entity>(serialised, out);

		for (const auto &script : serialised["scripts"].get<std::vector<std::string>>())
		{
			out->addScript(Script::createScript(out, Res::decode(script)).value());
		}
	}
}