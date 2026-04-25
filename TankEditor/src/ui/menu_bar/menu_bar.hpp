#pragma once
#include <nodes/ui/ui_node.hpp>


namespace Tank::Editor
{
	class EditorApp;
	/// @brief Abstract class which wraps an ImGui MainMenuBar.
	/// Multiple of these can exist at once - they just combine
	/// their renders.
	class _MenuBar : public UINode
	{
	private:
		void drawFile();
		void drawView();
		void drawSystem();

		bool spawnerMenuItem(const std::string &nodeName);
	protected:
		EditorApp &m_editor;
	public:
		_MenuBar(EditorApp &editor) : UINode("MenuBar"), m_editor(editor)
		{
			m_isEditorControlled = true;
		}

		void drawUI() override;
		virtual void drawMainMenuBar() = 0;
	};
}