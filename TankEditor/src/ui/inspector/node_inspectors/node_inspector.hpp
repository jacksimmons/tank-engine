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
		class Inspector_;

		class NodeInspector_Base
		{
		public:
			virtual void draw() = 0;
		};

		template <class T>
		class NodeInspector_ final : public NodeInspector_Base
		{
		private:
			T *m_node;
			Inspector_ *m_inspector;
		public:
			NodeInspector_(T *node, Inspector_ *inspector) :
				NodeInspector_Base(),
				m_node(node),
				m_inspector(inspector)
			{
			}

			void draw() override;
			const T &getNode() { return *m_node; };
		};
	}
}