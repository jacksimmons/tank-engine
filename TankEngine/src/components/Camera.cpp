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

		serialised["camera"] = serialise<Camera>(&in->camera);
		serialised["panSpd"] = in->panSpeed;
		serialised["rotSpd"] = in->rotationSpeed;

		return serialised;
	}

	template <>
	CameraComponent deserialise(const json &serialised)
	{
		CameraComponent cc {};

		cc.camera = deserialise<Camera>(serialised);
		cc.panSpeed = serialised["panSpd"];
		cc.rotationSpeed = serialised["rotSpd"];

		return cc;
	}
}
