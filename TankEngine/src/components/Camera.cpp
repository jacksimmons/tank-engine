#include "Camera.h"
#include <scene/SceneCamera.h>


namespace Tank
{
	// =======================
	//		Serialisation
	// =======================
	template <>
	json serialise<CameraComponent>(CameraComponent *in)
	{
		json serialised;

		serialised["camera"] = serialise<SceneCamera>(&in->Camera);
		serialised["panSpd"] = in->PanSpeed;
		serialised["rotSpd"] = in->RotationSpeed;

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
