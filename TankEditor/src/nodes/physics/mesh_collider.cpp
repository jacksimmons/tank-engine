#include "mesh_collider.hpp"
#include "log.hpp"
#include "static/time.hpp"


namespace Tank
{
	std::vector<MeshCollider*> MeshCollider::s_instances;

	MeshCollider::MeshCollider(const std::string &name, std::weak_ptr<Mesh> mesh)
		: Node(name), m_mesh(mesh)
	{
		s_instances.push_back(this);
	}

	MeshCollider::~MeshCollider()
	{
		auto it = std::find(s_instances.begin(), s_instances.end(), this);
		if (it != s_instances.end())
			s_instances.erase(it);
		else
			TE_CORE_CRITICAL("Destructor: MeshCollider was missing from s_instances.");
	}

	void MeshCollider::update()
	{
		if (!m_started) return Node::update();
		if (m_mesh.expired()) return;

		auto mesh = m_mesh.lock();

		// Ensure velocities list is correct size before starting
		float dt = Time::getFrameDelta();

		// Handle physics for all other physics bodies
		for (size_t i = 0; i < s_instances.size(); i++)
		{
			MeshCollider *col = s_instances[i];
			if (col == this) continue;

			
		}
	}
}