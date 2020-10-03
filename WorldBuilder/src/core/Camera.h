#ifndef Camera_h
#define Camera_h

#include "glm/gtx/transform.hpp"

namespace gps{

	enum class MOVE_DIRECTION{ 
		MOVE_FORWARD, 
		MOVE_BACKWARD, 
		MOVE_RIGHT, 
		MOVE_LEFT 
	};

	class Camera{
	public:
		Camera( ) = default;
		Camera(int  width, int height,const glm::vec3& cameraPosition = glm::vec3(0.0f, 5.0f, 5.0f),const glm::vec3& cameraTarget = glm::vec3(0.0f, 0.0f, -10.0f));

		void setProjection(int width, int height);

		void move(MOVE_DIRECTION direction, double timeStep);

		//yaw - rotate around y axis
		//pitch - rotate around x axis
		void rotate(double pitch, double yaw);

		glm::mat4 getViewMatrix( );
		glm::mat4 getPojection( )    { return projection; };
		glm::vec3 getCameraTarget( ) { return cameraTarget; };
		glm::vec3 getPosition( )     { return cameraPosition; };

	private:
		double m_yaw = -90.0f;
		double m_pitch = 0;

		glm::vec3 cameraPosition;
		glm::vec3 cameraTarget;
		glm::vec3 cameraDirection;
		glm::vec3 cameraRightDirection;

		glm::vec3 Up;
		glm::mat4 projection;
	};

}

#endif /* Camera_hpp */
