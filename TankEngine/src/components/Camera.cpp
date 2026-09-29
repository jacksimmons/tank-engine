#include "Camera.h"
#include <scene/Camera.h>


namespace Tank
{
	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise<CameraComponent>(CameraComponent *in)
	{
		json serialised;

		serialised["camera"] = serialise<Camera>(&in->Camera);
		serialised["panSpd"] = in->PanSpeed;
		serialised["rotSpd"] = in->RotationSpeed;

		return serialised;
	}

	template <>
	void deserialise<CameraComponent>(const json &serialised, CameraComponent *out)
	{
		deserialise(serialised["camera"], &out->Camera);

		out->PanSpeed = serialised["panSpd"];
		out->RotationSpeed = serialised["rotSpd"];
	}
}
