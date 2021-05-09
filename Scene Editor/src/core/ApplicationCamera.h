#pragma once
#include "core/Camera.h"

namespace SceneEditor{
	enum class MOVE_DIRECTION{
		MOVE_FORWARD,
		MOVE_BACKWARD,
		MOVE_RIGHT,
		MOVE_LEFT
	};

	class ApplicationCamera : public Camera{
	public:
		ApplicationCamera( ) = default;
		ApplicationCamera(uint32_t Width, uint32_t Height) : Camera{Width, Height} { };

		void Move(MOVE_DIRECTION Direction, double TimeStep);

		//yaw - rotate around y axis
		//pitch - rotate around x axis
		void Rotate(double Pitch, double Yaw);

	};
}


