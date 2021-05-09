#include "pch.h"
#include "Camera.h"

namespace SceneEditor{

#define CAMERA_SPEED 10.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 10000.0f
#define FOV 75.0f

	Camera::Camera( int Width, int Height) :
		m_CameraPosition(glm::vec3(0.0f, 5.0f, 15.0f)), m_CameraTarget(glm::vec3(0.0f, 0.0f, -10.0f)), m_Up(glm::vec3(0.0f, 1.0f, 0.0f))
	{
		m_CameraDirection = glm::normalize(m_CameraPosition - m_CameraTarget);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		m_CameraRightDirection = glm::normalize(glm::cross(up, m_CameraDirection));

		m_Projection = glm::perspective(glm::radians(FOV), (float)Width / (float)Height, NEAR_PLANE, FAR_PLANE);
	}

	glm::mat4 Camera::GetViewMatrix( ) const 
	{
		return  glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraTarget, m_Up);
	}

	void Camera::setProjection(int Width, int Height)
	{
		m_Projection = glm::perspective(glm::radians(FOV), (float)Width / (float)Height, NEAR_PLANE, FAR_PLANE);
	}

	void Camera::move(MOVE_DIRECTION Direction, double TimeStep)
	{
		float speed = CAMERA_SPEED * (float)TimeStep;

		switch(Direction){
			case MOVE_DIRECTION::MOVE_FORWARD:
				m_CameraPosition -= m_CameraDirection * speed;
				break;
			case MOVE_DIRECTION::MOVE_BACKWARD:
				m_CameraPosition += m_CameraDirection * speed;
				break;
			case MOVE_DIRECTION::MOVE_LEFT:
				m_CameraPosition -= m_CameraRightDirection * speed;
				break;
			case MOVE_DIRECTION::MOVE_RIGHT:
				m_CameraPosition += m_CameraRightDirection * speed;
				break;
		}
	}

	void Camera::rotate(double Pitch, double Yaw)
	{
		m_Pitch += Pitch;
		m_Yaw += Yaw;

		m_Pitch = (m_Pitch > 89.0f) ? 89.0f : m_Pitch;
		m_Pitch = (m_Pitch < -89.0f) ? -89.0f : m_Pitch;

		glm::vec3 front;
		front.x = (float)(cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
		front.y = (float)(sin(glm::radians(m_Pitch)));
		front.z = (float)(sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
		m_CameraTarget = glm::normalize(front);

		m_CameraRightDirection = glm::normalize(glm::cross(m_CameraTarget, glm::vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		m_Up = glm::normalize(glm::cross(m_CameraRightDirection, m_CameraTarget));
		m_CameraDirection = -m_CameraTarget;
	}
}