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
		SceneCamera camera;
		float panSpeed = 5;
		float rotationSpeed = 10;
		bool freeLookEnabled = true;

		CameraComponent() : camera({}) {};
		CameraComponent(CameraComponent &) = default;
	};
}