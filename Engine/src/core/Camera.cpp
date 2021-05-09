#include "pch.h"
#include "Camera.h"

namespace SceneEditor{

#define NEAR_PLANE 0.1f
#define FAR_PLANE 10000.0f
#define FOV 75.0f

	Camera::Camera( int Width, int Height) :
		m_CameraPosition(glm::vec3(0.0f, 8.0f, 15.0f)), m_CameraTarget(glm::vec3(0.0f, 0.0f, -10.0f)), m_Up(glm::vec3(0.0f, 1.0f, 0.0f))
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

	void Camera::SetProjection(int Width, int Height)
	{
		m_Projection = glm::perspective(glm::radians(FOV), (float)Width / (float)Height, NEAR_PLANE, FAR_PLANE);
	}
}