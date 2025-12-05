#pragma once
#include <events/event.hpp>
#include <events/event_manager.hpp>
#include <nodes/model.hpp>
#include <nodes/node.hpp>
#include <nodes/sprite.hpp>
#include <ui/file_dialog.hpp>


namespace Tank
{
	class ShaderContainer;

	namespace Editor
	{
		class _Inspector;

		class _NodeInspectorBase
		{
		public:
			virtual void draw() = 0;
		};

		template <class T>
		class _NodeInspector final : public _NodeInspectorBase
		{
		private:
			T *m_node;
			_Inspector *m_inspector;
		public:
			_NodeInspector(T *node, _Inspector *inspector) :
				_NodeInspectorBase(),
				m_node(node),
				m_inspector(inspector)
			{
				// Handle NodeInspector file dialog events
				Event<_FileDialog*, fs::path> *onItemSelected = EventManager::getEvent<_FileDialog*, fs::path>("FileDialog.ItemSelected");

				if constexpr (std::is_same_v<T, Model>)
				{
					onItemSelected->registerHandler([this](_FileDialog *dialog, const fs::path &path)
					{
						if (dialog->getName() == "Load Model File")
						{
							if (!path.has_filename()) return;
							m_node->setModelPath(path);
						}
					});
				}
				else if constexpr (std::is_same_v<T, Sprite>)
				{
					TE_CORE_INFO(dynamic_cast<Sprite*>(node)->getTexPath().string());
					onItemSelected->registerHandler([this](_FileDialog *dialog, const fs::path &path)
					{
						if (dialog->getName() == "Load Texture File")
						{
							// Only update the texture if user has selected a valid file
							if (!path.has_filename()) return;
							m_node->setTexPath(path);
						}
					});
				}
			}

			void draw() override;
			const T &getNode() { return *m_node; };
		};
	}
}