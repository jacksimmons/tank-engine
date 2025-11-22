#pragma once
#include <nodes/node.hpp>


namespace Tank
{
	namespace Editor
	{
		class _Inspector;

		template <class T>
		class _NodeInspector final
		{
		private:
			T *m_node;
			_Inspector *m_inspector;
		public:
			_NodeInspector(T *node, _Inspector *inspector) : m_node(node), m_inspector(inspector) {};

			const T &getNode() { return *m_node; };
			void draw();
		};
	}
}