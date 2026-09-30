#pragma once
#include <serialisation/Serialisation.h>


namespace Tank
{
	class Camera
	{
		friend class Scene;
		friend class CameraComponent;
		friend class Renderer;
		friend class Renderer2D;

		// Projection properties
		float m_cullNear;
		float m_cullFar;

		// Standard camera matrices
		glm::mat4 m_projection;
		glm::mat4 m_view;

		glm::mat4 m_rotation;
		glm::mat4 m_translation;

		// glm::look_at params
		glm::vec3 m_eye;
		glm::vec3 m_centre;
		glm::vec3 m_up;

	public:
		Camera() = default;
		Camera(glm::vec3 eye = { 0, 0, 3 }, glm::vec3 centre = { 0, 0, 0 }, glm::vec3 up = { 0, 1, 0 })
			: m_eye(eye), m_centre(centre), m_up(up) {}

		void updateProj() { m_projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, m_cullNear, m_cullFar); }

		void setPosition(const glm::vec3 &pos);
		void translate(const glm::vec3 &vec);

		void setRotation(const glm::quat &rot);
		void rotate(const glm::vec3 &vec);

		glm::vec3 getTransformedCentre() const;
		glm::vec3 getTransformedEye() const;
		glm::vec3 getTransformedUp() const;

		json serialise() const;
		void deserialise(const json &);
	};


	template <>
	json serialise<Camera>(Camera *);
	template <>
	void deserialise<Camera>(const json &, Camera *);
}