#pragma once
#include "nodes/Node.hpp"
#include "Mesh.hpp"


namespace Tank
{
	/// <summary>
	/// </summary>
	class MeshCollider : public Node
	{
	private:
		static std::vector<MeshCollider*> s_instances;
		std::weak_ptr<Mesh> m_mesh;
	public:
		MeshCollider(const std::string &name, std::weak_ptr<Mesh> mesh);
		virtual ~MeshCollider();

		void update() override;
	};
}