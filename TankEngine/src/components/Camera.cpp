#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Transformation.h>
#include <static/GlmSerialise.h>
#include "Camera.h"


namespace Tank
{
	CameraComponent::CameraComponent(glm::vec3 eye, glm::vec3 centre, glm::vec3 up)
	{
		// Create a perspective projection for this camera.
		CullNear = 0.1f;
		CullFar = 10000.0f;
		updateProj();
		View = glm::mat4(1.0f);
		Rotation = glm::mat4(1.0f);
		Translation = glm::mat4(1.0f);

		Eye = eye;
		Centre = centre;
		Up = up;

		PanSpeed = 10;
		RotationSpeed = 5;

		FreeLookEnabled = true;
	}


	void CameraComponent::setPosition(const glm::vec3 &pos)
	{
		Translation = glm::translate(glm::mat4(1.0f), pos);
	}

	void CameraComponent::translate(const glm::vec3 &vec)
	{
		Translation = glm::translate(Translation, glm::vec3(Rotation * glm::vec4(vec, 1.0f)));
	}

	void CameraComponent::setRotation(const glm::quat &rot)
	{
		Rotation = glm::mat4_cast(rot);
	}

	void CameraComponent::rotate(const glm::vec3 &vec)
	{
		glm::vec3 yAxis = glm::normalize(Rotation * glm::vec4(Up, 1.0f));
		glm::vec3 zAxis = glm::normalize(Rotation * glm::vec4(Centre - Eye, 1.0f));
		glm::vec3 xAxis = glm::cross(yAxis, zAxis);

		glm::quat rot = glm::vec3(vec.y, vec.x, vec.z);

		Rotation = glm::mat4_cast(rot) * Rotation;
	}

	glm::vec3 CameraComponent::getTransformedCentre() const
	{
		return glm::vec3(Translation * mat4::rotateAboutPoint(Centre, -Eye, Rotation) * glm::vec4(Centre, 1));
	}

	glm::vec3 CameraComponent::getTransformedEye() const
	{
		return glm::vec3(Translation * glm::vec4(Eye, 1));
	}

	glm::vec3 CameraComponent::getTransformedUp() const
	{
		return glm::vec3(Rotation * glm::vec4(Up, 1));
	}

	/// <summary>
	/// 1. Rotate center around the eye("Universe rotates around camera")
	/// (Translate center by - eye, then rotate it around origin with R)
	/// (Then translate the new center by eye)
	/// 2. Translate the eye with the displacement matrix(Camera movement)
	/// 3. Rotate the up vector around origin with R.
	/// Combining these gives the view matrix.
	/// </summary>
	void CameraComponent::update()
	{
		// Transformed vector
		glm::vec3 t_centre = getTransformedCentre();
		glm::vec3 t_eye = getTransformedEye();
		glm::vec3 t_up = getTransformedUp();
		m_V = getComponent<TransformComponent>().getWorldModelMatrix() * glm::lookAt(t_eye, t_centre, t_up);

		Node::update();
	}


	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise(CameraComponent *in)
	{
		json serialised;

		serialised["view"] = mat4::serialise(in->View);
		serialised["rotation"] = mat4::serialise(in->Rotation);
		serialised["translation"] = mat4::serialise(in->Translation);

		serialised["eye"] = vec3::serialise(in->Eye);
		serialised["centre"] = vec3::serialise(in->Centre);
		serialised["up"] = vec3::serialise(in->Up);

		serialised["panSpd"] = in->PanSpeed;
		serialised["rotSpd"] = in->RotationSpeed;

		serialised["cullNear"] = in->CullNear;
		serialised["cullFar"] = in->CullFar;

		return serialised;
	}

	template <>
	void deserialise(const json &serialised, CameraComponent *out)
	{
		out->View = mat4::deserialise(serialised["view"]);
		out->Rotation = mat4::deserialise(serialised["rotation"]);
		out->Translation = mat4::deserialise(serialised["translation"]);

		out->Eye = vec3::deserialise(serialised["eye"]);
		out->Centre = vec3::deserialise(serialised["centre"]);
		out->Up = vec3::deserialise(serialised["up"]);

		out->PanSpeed = serialised["panSpd"];
		out->RotationSpeed = serialised["rotSpd"];

		out->CullNear = serialised["cullNear"];
		out->CullFar = serialised["cullFar"];
		out->updateProj();
	}
}