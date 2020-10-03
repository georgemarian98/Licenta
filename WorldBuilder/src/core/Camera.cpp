#include "3DWBpch.h"
#include "Camera.h"

#define CAMERA_SPEED 10.0f

namespace gps{

	Camera::Camera( int width, int height,const glm::vec3& cameraPosition,const glm::vec3& cameraTarget) :
		cameraPosition(cameraPosition), cameraTarget(cameraTarget), Up(glm::vec3(0.0f, 1.0f, 0.0f))
	{
		cameraDirection = glm::normalize(cameraPosition - cameraTarget);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		cameraRightDirection = glm::normalize(glm::cross(up, cameraDirection));

		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
	}

	glm::mat4 Camera::getViewMatrix( )
	{
		return  glm::lookAt(cameraPosition, cameraPosition + cameraTarget, Up);
	}

	void Camera::setProjection(int width, int height)
	{
		projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
	}

	void Camera::move(gps::MOVE_DIRECTION direction, double timeStep)
	{
		float speed = CAMERA_SPEED * (float)timeStep;

		switch(direction){
			case MOVE_DIRECTION::MOVE_FORWARD:
				cameraPosition -= cameraDirection * speed;
				break;
			case MOVE_DIRECTION::MOVE_BACKWARD:
				cameraPosition += cameraDirection * speed;
				break;
			case MOVE_DIRECTION::MOVE_LEFT:
				cameraPosition -= cameraRightDirection * speed;
				break;
			case MOVE_DIRECTION::MOVE_RIGHT:
				cameraPosition += cameraRightDirection * speed;
				break;
		}
	}

	void Camera::rotate(double pitch, double yaw)
	{
		m_pitch += pitch;
		m_yaw += yaw;

		m_pitch = (m_pitch > 89.0f) ? 89.0f : m_pitch;
		m_pitch = (m_pitch < -89.0f) ? -89.0f : m_pitch;

		glm::vec3 front;
		front.x = (float)(cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
		front.y = (float)(sin(glm::radians(m_pitch)));
		front.z = (float)(sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch)));
		cameraTarget = glm::normalize(front);

		cameraRightDirection = glm::normalize(glm::cross(cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(cameraRightDirection, cameraTarget));
		cameraDirection = -cameraTarget;
	}
}