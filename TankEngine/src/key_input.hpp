#pragma once
struct GLFWwindow;
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
	class TANK_API KeyInput
	{
	private:
		static std::vector<KeyInput *> s_instances;
		std::map<int, KeyState> m_keys;

		void setKeyState(int key, KeyState state);
	public:
		static void callback(GLFWwindow *win, int key, int scancode, int action, int mods);
		
		KeyInput(const std::vector<int> &monitoredKeys);
		~KeyInput();

		KeyState getKeyState(int key);
		void update();
	};
}