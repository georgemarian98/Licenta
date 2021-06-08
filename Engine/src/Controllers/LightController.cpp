#include "pch.h"
#include "LightController.h"

namespace SceneEditor {

glm::mat4 LightController::GetSpaceMatrix()
{
	glm::mat4 lightView = glm::lookAt(m_Position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	return m_LightProjection * lightView;
}
}

