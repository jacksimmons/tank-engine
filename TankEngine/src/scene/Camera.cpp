#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <static/GlmSerialise.h>
#include <Transformation.h>
#include "Camera.h"


namespace Tank
{
	Camera::Camera(glm::vec3 eye, glm::vec3 centre, glm::vec3 up)
	{
		// Create a perspective projection for this camera.
		m_cullNear = 0.1f;
		m_cullFar = 10000.0f;
		updateProj();
		m_view = glm::mat4(1.0f);
		m_rotation = glm::mat4(1.0f);
		m_translation = glm::mat4(1.0f);

		m_eye = eye;
		m_centre = centre;
		m_up = up;
	}


	void Camera::setPosition(const glm::vec3 &pos)
	{
		m_translation = glm::translate(glm::mat4(1.0f), pos);
	}

	void Camera::translate(const glm::vec3 &vec)
	{
		m_translation = glm::translate(m_translation, glm::vec3(m_rotation * glm::vec4(vec, 1.0f)));
	}

	void Camera::setRotation(const glm::quat &rot)
	{
		m_rotation = glm::mat4_cast(rot);
	}

	void Camera::rotate(const glm::vec3 &vec)
	{
		glm::vec3 yAxis = glm::normalize(m_rotation * glm::vec4(m_up, 1.0f));
		glm::vec3 zAxis = glm::normalize(m_rotation * glm::vec4(m_centre - m_eye, 1.0f));
		glm::vec3 xAxis = glm::cross(yAxis, zAxis);

		glm::quat rot = glm::vec3(vec.y, vec.x, vec.z);

		m_rotation = glm::mat4_cast(rot) * m_rotation;
	}

	glm::vec3 Camera::getTransformedCentre() const
	{
		return glm::vec3(m_translation * mat4::rotateAboutPoint(m_centre, -m_eye, m_rotation) * glm::vec4(m_centre, 1));
	}

	glm::vec3 Camera::getTransformedEye() const
	{
		return glm::vec3(m_translation * glm::vec4(m_eye, 1));
	}

	glm::vec3 Camera::getTransformedUp() const
	{
		return glm::vec3(m_rotation * glm::vec4(m_up, 1));
	}


	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise<Camera>(Camera *in)
	{
		return in->serialise();
	}
	json Camera::serialise() const
	{
		json serialised;

		serialised["view"] = mat4::serialise(m_view);
		serialised["rotation"] = mat4::serialise(m_rotation);
		serialised["translation"] = mat4::serialise(m_translation);

		serialised["eye"] = vec3::serialise(m_eye);
		serialised["centre"] = vec3::serialise(m_centre);
		serialised["up"] = vec3::serialise(m_up);

		serialised["cullNear"] = m_cullNear;
		serialised["cullFar"] = m_cullFar;
	}

	template <>
	void deserialise<Camera>(const json &serialised, Camera *out)
	{
		out->deserialise(serialised);
	}
	void Camera::deserialise(const json &serialised)
	{
		m_view = mat4::deserialise(serialised["view"]);
		m_rotation = mat4::deserialise(serialised["rotation"]);
		m_translation = mat4::deserialise(serialised["translation"]);

		m_eye = vec3::deserialise(serialised["eye"]);
		m_centre = vec3::deserialise(serialised["centre"]);
		m_up = vec3::deserialise(serialised["up"]);

		m_cullNear = serialised["cullNear"];
		m_cullFar = serialised["cullFar"];
		updateProj();
	}
}