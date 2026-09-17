#pragma once
#include <events/Event.hpp>
#include <events/EventManager.hpp>
#include <nodes/Node.hpp>
#include <nodes/Model.hpp>
#include <nodes/Sprite.hpp>
#include <nodes/Audio.hpp>
#include <ui/FileDialog.hpp>


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
