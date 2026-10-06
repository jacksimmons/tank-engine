#pragma once
#include <glm/gtx/quaternion.hpp>
#include <serialisation/Serialisation.h>
#include <scene/SceneCamera.h>
#include <Transformation.h>


namespace Tank
{
	class SceneCamera;


	struct CameraComponent
	{
		SceneCamera Camera;
		float PanSpeed = 5;
		float RotationSpeed = 10;
		bool FreeLookEnabled = true;

		CameraComponent() = default;
		CameraComponent(CameraComponent &) = default;
	};


	template <>
	json serialise<CameraComponent>(CameraComponent *);
	template <>
	void deserialise<CameraComponent>(const json &, CameraComponent *);
}