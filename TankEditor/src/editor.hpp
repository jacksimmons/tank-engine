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
	class ProjectMenuBar_;
	class EditorApp : public Application
	{
	private:
		static std::vector<std::string> s_windowNames;
		
		// Root for initial UI
		std::unique_ptr<Node> m_initUI;
		// Root for project UI
		std::unique_ptr<Node> m_projectUI;
		// Root for scene nodes
		std::unique_ptr<Node> m_sceneRoot;

		// Keyboard input for the Editor only.
		std::unique_ptr<KeyInput> m_editorInput;
		std::unique_ptr<Project> m_project;
	public:
		EditorApp();
		~EditorApp();

		const Project &getProject() const { return *m_project; }
		Node &getProjRoot() const { return *m_sceneRoot; }
		void setScene(std::unique_ptr<Scene> scene);
	protected:
		virtual void step() override;
		virtual void uiStep() override;
		virtual void handleKeyInput();
	};
}
