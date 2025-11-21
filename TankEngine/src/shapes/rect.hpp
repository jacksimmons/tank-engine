#pragma once


namespace Tank
{
	/// <summary>
	/// A shape which starts at its origin and extends by its size.
	/// </summary>
	class Rect
	{
	private:
		glm::vec3 m_origin;
		glm::vec3 m_size;
	public:
		const glm::vec3 &getOrigin() { return m_origin; }
	};
}