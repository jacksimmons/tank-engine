#pragma once
#include <events/event.hpp>
#include <events/event_manager.hpp>
#include <nodes/node.hpp>
#include <nodes/model.hpp>
#include <nodes/sprite.hpp>
#include <nodes/audio.hpp>
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
			}

			void draw() override;
			const T &getNode() { return *m_node; };
		};
	}
}