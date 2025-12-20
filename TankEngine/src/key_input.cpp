#include <GLFW/glfw3.h>
#include "key_input.hpp"
#include <log.hpp>


namespace Tank
{
	// Initialisation of s_instances must be done in cpp.
	std::vector<KeyInput *> KeyInput::s_instances;


	KeyInput::KeyInput(const std::vector<int> &monitoredKeys)
	{
		for (int key : monitoredKeys)
		{
			m_keys[key] = KeyState::NotPressed;
		}

		s_instances.push_back(this);
	}


	KeyInput::~KeyInput()
	{
		s_instances.erase(std::remove(s_instances.begin(), s_instances.end(), this), s_instances.end());
	}


	void KeyInput::callback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		// Send the key event to all KeyInput instances.
		for (KeyInput *ki : s_instances)
		{
			KeyState next;

			// Either set state to Held, Pressed or Released (Released decays into NotPressed)
			// Rules:
			// - If somehow Pressed/Released happen at once, Released takes precedence for safety.
			if (action == GLFW_PRESS)
			{
				next = KeyState::Pressed;
			}
			else if (action == GLFW_RELEASE)
			{
				next = KeyState::Released;
			}
			else
			{
				return;
			}

			ki->setKeyState(key, next);
		}
	}


	void KeyInput::setKeyState(int key, KeyState state)
	{
		auto it = m_keys.find(key);
		if (it != m_keys.end())
		{
			m_keys[key] = state;
		}
	}


	KeyState KeyInput::getKeyState(int key)
	{
		KeyState result = KeyState::NotPressed;

		// if isenabled

		// Find the key in the keys vector
		auto it = m_keys.find(key);

		// If the key was found...
		if (it != m_keys.end())
			result = m_keys[key];

		// endif
		return result;
	}


	/// <summary>
	/// Frame-by-frame state updates.
	/// </summary>
	void KeyInput::update()
	{
		for (KeyInput *ki : s_instances)
		{
			// For all input keys...
			for (auto it = m_keys.begin(); it != m_keys.end(); ++it)
			{
				// Decay Pressed states into Held states
				if (it->second == KeyState::Pressed)
				{
					it->second = KeyState::Held;
				}

				// Decay Released states into NotPressed states.
				else if (it->second == KeyState::Released)
				{
					it->second = KeyState::NotPressed;
				}
			}
		}
	}
}