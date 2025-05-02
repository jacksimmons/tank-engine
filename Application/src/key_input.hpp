#pragma once

#include "nodes/camera.hpp"


struct GLFWWindow;
namespace Tank
{
	enum class KeyState
	{
		Pressed,
		Held,
		Released,
		NotPressed
	};


	/// <summary>
	/// Class for handling keyboard inputs. Must be provided with all monitored keys in its
	/// constructor.
	/// </summary>
	class KeyInput
	{
	private:
		static std::vector<KeyInput *> s_instances;
		std::map<int, KeyState> m_keys;

		static void callback(GLFWwindow *win, int key, int scancode, int action, int mods);
		void setKeyState(int key, KeyState state);
	public:
		KeyInput(std::vector<int> monitoredKeys);
		~KeyInput();

		static void setupKeyInputs(GLFWwindow *window);
		KeyState getKeyState(int key);
		void update();
	};
}