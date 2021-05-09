#pragma once

#include "glm/gtx/transform.hpp"

namespace SceneEditor{

	class Camera{
	public:
		Camera( ) = default;
		Camera(int  Width, int Height);

		void SetProjection(int Width, int Height);

		glm::mat4 GetViewMatrix( )   const ;
		glm::mat4 GetPojection( )    const { return m_Projection; };
		glm::vec3 GetPosition( )     const { return m_CameraPosition; };

	protected:
		double m_Yaw = -90.0f;
		double m_Pitch = 0;

		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraTarget;
		glm::vec3 m_CameraDirection;
		glm::vec3 m_CameraRightDirection;

		glm::vec3 m_Up;
		glm::mat4 m_Projection;
	};
}

#endif /* Camera_hpp */
