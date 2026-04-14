namespace Tank
{
	class Node;
	class Scene;
}


namespace Tank::Editor
{
	/// @brief Contains the Node hierarchy of the Editor.
	/// Is controlled by the Editor, and persists between scenes.
	class EditorRoot
	{
	private:
		static EditorRoot s_instance;
		std::unique_ptr<Node> m_root;
	public:
		static void setRoot(std::unique_ptr<Node> root)
		{
			s_instance.m_root = std::move(root);
		}
		static Node& getRoot() { return *(s_instance.m_root); }
		static bool isReady() { return s_instance.m_root != nullptr; }


		static void setScene(std::unique_ptr<Scene> scene);
	};
}