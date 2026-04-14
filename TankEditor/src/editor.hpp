#pragma once
#include <application.hpp>


class ImGuiIO;
namespace Tank
{
	class Node;
	class Scene;
	class Project;
	class KeyInput;
}
namespace Tank::Editor
{
	class ProjectMenuBar;
	class EditorApp : public Application
	{
	private:
		static std::vector<std::string> s_windowNames;
			
		std::unique_ptr<Node> m_initUI;
		// Keyboard input for the Editor only.
		std::unique_ptr<KeyInput> m_editorInput;
		std::unique_ptr<Project> m_project;
	public:
		EditorApp();
		~EditorApp();

		const Project &getProject() const { return *m_project; }
	protected:
		virtual void step() override;
		virtual void uiStep() override;
		virtual void handleKeyInput();
	};
}
