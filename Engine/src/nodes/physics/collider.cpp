#include "collider.hpp"
#include "log.hpp"


namespace Tank
{
	std::vector<Collider*> Collider::s_instances;

	Collider::Collider(const std::string &name, std::unique_ptr<CollisionShape> shape)
		: Node(name), m_shape(std::move(shape))
	{
		s_instances.push_back(this);
	}

	Collider::~Collider()
	{
		auto it = std::find(s_instances.begin(), s_instances.end(), this);
		if (it != s_instances.end())
			s_instances.erase(it);
		else
			TE_CORE_CRITICAL("Destructor: Collider was missing from s_instances.");
	}

	void Collider::update()
	{
		// Check for any collisions with other colliders
		for (Collider *collider : s_instances)
		{
			if (collider == this) continue;
			if (m_shape->contains(collider->getTransform()->getLocalTranslation()))
			{
				TE_CORE_INFO(std::format("Collision: Offender {}, Recipient {}", collider->getName(), getName()));
			}
		}
	}
}