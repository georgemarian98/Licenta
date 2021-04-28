#ifndef Camera_h
#define Camera_h

#include "glm/gtx/transform.hpp"

namespace SceneEditor{

	enum class MOVE_DIRECTION{ 
		MOVE_FORWARD, 
		MOVE_BACKWARD, 
		MOVE_RIGHT, 
		MOVE_LEFT 
	};

	class Camera{
	public:
		Camera( ) = default;
		Camera(int  Width, int Height,const glm::vec3& CameraPosition = glm::vec3(0.0f, 5.0f, 5.0f),const glm::vec3& CameraTarget = glm::vec3(0.0f, 0.0f, -10.0f));

		void setProjection(int Width, int Height);

		void move(MOVE_DIRECTION Direction, double TimeStep);

		//yaw - rotate around y axis
		//pitch - rotate around x axis
		void rotate(double Pitch, double Yaw);

		glm::mat4 GetViewMatrix( )   const ;
		glm::mat4 GetPojection( )    const { return m_Projection; };
		glm::vec3 GetCameraTarget( ) const { return m_CameraTarget; };
		glm::vec3 GetPosition( )     const { return m_CameraPosition; };

	private:
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
