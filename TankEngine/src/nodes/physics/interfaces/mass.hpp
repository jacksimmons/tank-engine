#pragma once
namespace Tank
{
	/// <summary>
	/// A mass; an object with a mass property.
	/// </summary>
	class TANK_API IMass
	{
	private:
		float m_mass;
	public:
		IMass(float mass) : m_mass(mass)
		{
		};
	public:
		virtual ~IMass() = default;

		float getMass() const { return m_mass; }
	};
}