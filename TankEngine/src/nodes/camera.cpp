#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "transformation.hpp"
#include "static/glm_serialise.hpp"
#include "nodes/camera.hpp"


namespace Tank
{
	json Camera::serialise()
	{
		json serialised = Node::serialise();

		serialised["view"] = mat4::serialise(m_V);
		serialised["rotation"] = mat4::serialise(m_R);
		serialised["translation"] = mat4::serialise(m_T);

		serialised["eye"] = vec3::serialise(m_eye);
		serialised["centre"] = vec3::serialise(m_centre);
		serialised["up"] = vec3::serialise(m_up);

		serialised["panSpd"] = m_panSpeed;
		serialised["rotSpd"] = m_rotSpeed;

		serialised["cullNear"] = m_cullNear;
		serialised["cullFar"] = m_cullFar;

		return serialised;
	}


	void Camera::deserialise(const json &serialised, Camera **targetPtr)
	{
		if (!(*targetPtr)) *targetPtr = new Camera();

		Camera *camera = *targetPtr;
		camera->m_V = mat4::deserialise(serialised["view"]);
		camera->m_R = mat4::deserialise(serialised["rotation"]);
		camera->m_T = mat4::deserialise(serialised["translation"]);

		camera->m_eye = vec3::deserialise(serialised["eye"]);
		camera->m_centre = vec3::deserialise(serialised["centre"]);
		camera->m_up = vec3::deserialise(serialised["up"]);

		camera->m_panSpeed = serialised["panSpd"];
		camera->m_rotSpeed = serialised["rotSpd"];

		camera->m_cullNear = serialised["cullNear"];
		camera->m_cullFar = serialised["cullFar"];
		camera->updateProj();

		Node *target = *targetPtr;
		Node::deserialise(serialised, &target);
	}


	Camera::Camera(const std::string &name, glm::vec3 eye, glm::vec3 centre, glm::vec3 up) :
		Node(name)
	{
		m_type = "Camera";

		// Create a perspective projection for this camera.
		m_cullNear = 0.1f;
		m_cullFar = 10000.0f;
		updateProj();
		m_V = glm::mat4(1.0f);
		m_R = glm::mat4(1.0f);
		m_T = glm::mat4(1.0f);

		m_eye = eye;
		m_centre = centre;
		m_up = up;

		m_panSpeed = 10;
		m_rotSpeed = 5;

		m_freeLook = true;
	}


	void Camera::setPosition(glm::vec3 pos)
	{
		m_T = glm::translate(glm::mat4(1.0f), pos);
	}

	void Camera::translate(glm::vec3 vec)
	{
		m_T = glm::translate(m_T, glm::vec3(m_R * glm::vec4(vec, 1.0f)));
	}

	void Camera::setRotation(glm::quat rot)
	{
		m_R = glm::mat4_cast(rot);
	}

	void Camera::rotate(glm::vec3 vec)
	{
		glm::vec3 yAxis = glm::normalize(m_R * glm::vec4(m_up, 1.0f));
		glm::vec3 zAxis = glm::normalize(m_R * glm::vec4(m_centre - m_eye, 1.0f));
		glm::vec3 xAxis = glm::cross(yAxis, zAxis);

		glm::quat rot = glm::vec3(vec.y, vec.x, vec.z);

		m_R = glm::mat4_cast(rot) * m_R;
	}

	glm::vec3 Camera::getTransformedCentre() const
	{
		return glm::vec3(m_T * mat4::rotateAboutPoint(m_centre, -m_eye, m_R) * glm::vec4(m_centre, 1));
	}

	glm::vec3 Camera::getTransformedEye() const
	{
		return glm::vec3(m_T * glm::vec4(m_eye, 1));
	}

	glm::vec3 Camera::getTransformedUp() const
	{
		return glm::vec3(m_R * glm::vec4(m_up, 1));
	}

	/// <summary>
	/// 1. Rotate center around the eye("Universe rotates around camera")
	/// (Translate center by - eye, then rotate it around origin with R)
	/// (Then translate the new center by eye)
	/// 2. Translate the eye with the displacement matrix(Camera movement)
	/// 3. Rotate the up vector around origin with R.
	/// Combining these gives the view matrix.
	/// </summary>
	void Camera::update()
	{
		// Transformed vector
		glm::vec3 t_centre = getTransformedCentre();
		glm::vec3 t_eye = getTransformedEye();
		glm::vec3 t_up = getTransformedUp();
		m_V = getTransform()->getWorldModelMatrix() * glm::lookAt(t_eye, t_centre, t_up);
	}
}