#pragma once
#include <glm/gtx/quaternion.hpp>
#include <serialisation/Serialisation.h>
#include <scene/Camera.h>
#include <Transformation.h>


namespace Tank
{
	class Camera;


	struct CameraComponent
	{
		Camera camera;
		float panSpeed = 5;
		float rotationSpeed = 10;
		bool freeLookEnabled = true;

		CameraComponent() : camera({}) {};
		CameraComponent(CameraComponent &) = default;
	};


	template <>
	json serialise<CameraComponent>(CameraComponent *);
	template <>
	CameraComponent deserialise(const json &);
}