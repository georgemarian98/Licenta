#pragma once
#include "glm/glm.hpp"

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture{
    uint32_t id = 0;
    std::string type;
    std::string path;

    Texture(uint32_t id, std::string type, std::string path) :id(id), type(type), path(path) { };
};