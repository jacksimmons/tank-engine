#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <serialisation/GlmSerialisation.h>
#include <serialisation/Serialisation.h>
#include <Transformation.h>
#include "SceneCamera.h"


namespace Tank
{
	SceneCamera::SceneCamera(glm::vec3 eye, glm::vec3 centre, glm::vec3 up)
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


	void SceneCamera::setPosition(const glm::vec3 &pos)
	{
		m_translation = glm::translate(glm::mat4(1.0f), pos);
	}

	void SceneCamera::translate(const glm::vec3 &vec)
	{
		m_translation = glm::translate(m_translation, glm::vec3(m_rotation * glm::vec4(vec, 1.0f)));
	}

	void SceneCamera::setRotation(const glm::quat &rot)
	{
		m_rotation = glm::mat4_cast(rot);
	}

	void SceneCamera::rotate(const glm::vec3 &vec)
	{
		glm::vec3 yAxis = glm::normalize(m_rotation * glm::vec4(m_up, 1.0f));
		glm::vec3 zAxis = glm::normalize(m_rotation * glm::vec4(m_centre - m_eye, 1.0f));
		glm::vec3 xAxis = glm::cross(yAxis, zAxis);

		glm::quat rot = glm::vec3(vec.y, vec.x, vec.z);

		m_rotation = glm::mat4_cast(rot) * m_rotation;
	}

	glm::vec3 SceneCamera::getTransformedCentre() const
	{
		return glm::vec3(m_translation * mat4::rotateAboutPoint(m_centre, -m_eye, m_rotation) * glm::vec4(m_centre, 1));
	}

	glm::vec3 SceneCamera::getTransformedEye() const
	{
		return glm::vec3(m_translation * glm::vec4(m_eye, 1));
	}

	glm::vec3 SceneCamera::getTransformedUp() const
	{
		return glm::vec3(m_rotation * glm::vec4(m_up, 1));
	}


	// =======================
	//		Serialisation
	// =======================
	template <>
	json Serialisation::serialise<SceneCamera>(SceneCamera *in)
	{
		json serialised;

		serialised["view"] = mat4::serialise(in->m_view);
		serialised["rotation"] = mat4::serialise(in->m_rotation);
		serialised["translation"] = mat4::serialise(in->m_translation);

		serialised["eye"] = vec3::serialise(in->m_eye);
		serialised["centre"] = vec3::serialise(in->m_centre);
		serialised["up"] = vec3::serialise(in->m_up);

		serialised["cullNear"] = in->m_cullNear;
		serialised["cullFar"] = in->m_cullFar;

		return serialised;
	}

	template <>
	SceneCamera Serialisation::deserialise<SceneCamera>(const json &serialised)
	{
		SceneCamera cam
		{
			vec3::deserialise(serialised["eye"]),
			vec3::deserialise(serialised["centre"]),
			vec3::deserialise(serialised["up"])
		};

		cam.m_view = mat4::deserialise(serialised["view"]);
		cam.m_rotation = mat4::deserialise(serialised["rotation"]);
		cam.m_translation = mat4::deserialise(serialised["translation"]);

		cam.m_cullNear = serialised["cullNear"];
		cam.m_cullFar = serialised["cullFar"];
		cam.updateProj();

		cam.deserialise(serialised);
		return cam;
	}
}