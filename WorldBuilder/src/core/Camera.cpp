#include "pch.h"
#include "Camera.h"

#define CAMERA_SPEED 10.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 10000.0f


Camera::Camera( int Width, int Height,const glm::vec3& CameraPosition,const glm::vec3& CameraTarget) :
	m_CameraPosition(CameraPosition), m_CameraTarget(CameraTarget), m_Up(glm::vec3(0.0f, 1.0f, 0.0f))
{
	m_CameraDirection = glm::normalize(CameraPosition - CameraTarget);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	m_CameraRightDirection = glm::normalize(glm::cross(up, m_CameraDirection));

	//float aspect_ratio = (float)Width / (float)Height;
	//float orthoLeft = -m_OrthographicSize * aspect_ratio * 0.5f;
	//float orthoRight = m_OrthographicSize * aspect_ratio * 0.5f;
	//float orthoBottom = -m_OrthographicSize * 0.5f;
	//float orthoTop = m_OrthographicSize * 0.5f;

	//m_Projection = glm::ortho(orthoLeft, orthoRight,
	//						  orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
	m_Projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, NEAR_PLANE, FAR_PLANE);
}

glm::mat4 Camera::GetViewMatrix( ) const 
{
	return  glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraTarget, m_Up);
}

void Camera::setProjection(int Width, int Height)
{
	m_Projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, NEAR_PLANE, FAR_PLANE);
	//float aspect_ratio = (float)Width / (float)Height;
	//float orthoLeft = -m_OrthographicSize * aspect_ratio * 0.5f;
	//float orthoRight = m_OrthographicSize * aspect_ratio * 0.5f;
	//float orthoBottom = -m_OrthographicSize * 0.5f;
	//float orthoTop = m_OrthographicSize * 0.5f;

	//m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
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