#pragma once
#include <nodes/node.hpp>


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
			_NodeInspector(T *node, _Inspector *inspector) : _NodeInspectorBase(), m_node(node), m_inspector(inspector) {};

			void draw() override;
			const T &getNode() { return *m_node; };
		};
	}
}