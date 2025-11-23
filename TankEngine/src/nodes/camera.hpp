#pragma once
#include <glm/gtx/quaternion.hpp>
#include "transformation.hpp"
#include "nodes/node.hpp"
#include "nodes/interfaces/editor_only.hpp"


namespace Tank
{
	namespace Reflect { class NodeFactory; }


	class TANK_API Camera : public Node
	{
	public:
		virtual json serialise() override;
		virtual void deserialise(const json &ser) override;

	private:
		friend class Reflect::NodeFactory;
		// Projection properties
		float m_cullNear;
		float m_cullFar;

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

		bool m_freeLook;
		float m_panSpeed;
		float m_rotSpeed;
	public:
		Camera(const std::string &name = "Camera",
			glm::vec3 eye = { 0, 0, 3 },
			glm::vec3 centre = { 0, 0, 0 },
			glm::vec3 up = { 0, 1, 0 }
		);

		void setCullNear(float distance) {
			m_cullNear = distance;
			updateProj();
		}
		float getCullNear() { return m_cullNear; }
		void setCullFar(float distance) {
			m_cullFar = distance;
			updateProj();
		}
		float getCullFar() { return m_cullFar; }
		
		void updateProj() { m_P = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, m_cullNear, m_cullFar); }
		glm::mat4 getView() const noexcept { return m_V; }
		glm::mat4 getProj() const noexcept { return m_P; }

		void setPosition(glm::vec3 pos);
		void translate(glm::vec3 vec);
		
		void setRotation(glm::quat rot);
		void rotate(glm::vec3 vec);
		
		glm::vec3 getTransformedCentre() const;
		glm::vec3 getTransformedEye() const;
		glm::vec3 getTransformedUp() const;

		void setPanSpeed(float speed) noexcept { m_panSpeed = speed; }
		float getPanSpeed() const noexcept { return m_panSpeed; }

		void setRotSpeed(float speed) noexcept { m_rotSpeed = speed; }
		float getRotSpeed() const noexcept { return m_rotSpeed; }

		void setFreeLook(bool freeLook) noexcept { m_freeLook = freeLook; }
		bool getFreeLook() const noexcept { return m_freeLook; }

		void update() override;
	};
}