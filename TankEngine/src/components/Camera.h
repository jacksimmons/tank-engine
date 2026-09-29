#pragma once
#include <glm/gtx/quaternion.hpp>
#include <Transformation.h>
#include <serialisation/Serialisation.h>


namespace Tank
{
	struct CameraComponent
	{
		// Projection properties
		float CullNear;
		float CullFar;

		// Standard camera matrices
		glm::mat4 Projection;
		glm::mat4 View;

		glm::mat4 Rotation;
		glm::mat4 Translation;

		// glm::look_at params
		glm::vec3 Eye;
		glm::vec3 Centre;
		glm::vec3 Up;

		bool FreeLookEnabled;
		float PanSpeed;
		float RotationSpeed;

		CameraComponent() = default;
		CameraComponent(
			glm::vec3 eye = { 0, 0, 3 },
			glm::vec3 centre = { 0, 0, 0 },
			glm::vec3 up = { 0, 1, 0 }
		) : Eye(eye), Centre(centre), Up(up) {}

		void updateProj() { Projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, CullNear, CullFar); }

		void setPosition(const glm::vec3 &pos);
		void translate(const glm::vec3 &vec);
		
		void setRotation(const glm::quat &rot);
		void rotate(const glm::vec3 &vec);
		
		glm::vec3 getTransformedCentre() const;
		glm::vec3 getTransformedEye() const;
		glm::vec3 getTransformedUp() const;
	};


	template <>
	json serialise<CameraComponent>(CameraComponent *);
	template <>
	void deserialise<CameraComponent>(const json &, CameraComponent *);
}