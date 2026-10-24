#pragma once
#include <imgui.h>
#include <entity/EditorEntity.h>


namespace Tank::Editor
{
	struct WindowOpts
	{
		ImGuiWindowFlags flags;
		bool closeable;
		bool autoScroll;
	};


	/// <summary>
	/// ImGui window renderer component.
	/// </summary>
	class _Window : public EditorEntity
	{
	protected:
		_Window(entt::entity handle, Scene *ecs, WindowOpts opts);
		WindowOpts m_opts;
	public:
		virtual ~_Window() = default;

		/// <summary>
		/// Inherited draw method from Node. Calls drawPanel.
		/// </summary>
		virtual void drawUI() override;

		/// <summary>
		/// [ABSTRACT]
		/// All draw logic for the window goes in here.
		/// This function is called between an ImGui::Begin
		/// and an ImGui::End.
		/// </summary>
		virtual void drawPanel() = 0;
	};
}
