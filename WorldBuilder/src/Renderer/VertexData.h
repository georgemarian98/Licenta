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

struct Transforms{
	glm::vec3 Translation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);

	Transforms( ) { };
	Transforms(glm::vec3& Translation, glm::vec3& Scale, glm::vec3& Rotation) : Translation(Translation), Scale(Scale), Rotation(Rotation) {};
};