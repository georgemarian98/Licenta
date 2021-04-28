#include "pch.h"

std::ostream& operator<<(std::ostream& g, glm::vec3 vec)
{
	g << vec.x << ", " << vec.y << ", " << vec.z;
	return g;
}