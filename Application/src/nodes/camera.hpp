#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "nodes/node.hpp"
#include "nodes/hierarchy.hpp"
#include "transformation.hpp"


namespace Tank
{
	class Camera : public Node
	{
	private:
		// Projection matrix
		glm::mat4 m_P;
		// View matrix
		glm::mat4 m_V;
		// Camera rotation
		glm::mat4 m_R;
		// Camera translation
		glm::mat4 m_T;

		// glm::look_at params
		glm::vec3 m_eye;
		glm::vec3 m_centre;
		glm::vec3 m_up;
	public:
		Camera(std::string name,
			glm::vec3 eye = { 0, 0, 3 },
			glm::vec3 centre = { 0, 0, 0 },
			glm::vec3 up = { 0, 1, 0 });
		glm::mat4 getView() const noexcept { return m_V; }
		glm::mat4 getProj() const noexcept { return m_P; }
		void setPosition(glm::vec3 pos);
		void translate(glm::vec3 vec);
		void setRotation(glm::quat rot);
		void rotate(glm::vec3 vec);
		glm::vec3 getTransformedCentre() const;
		glm::vec3 getTransformedEye() const;
		glm::vec3 getTransformedUp() const;
		void update() override;
	};
}