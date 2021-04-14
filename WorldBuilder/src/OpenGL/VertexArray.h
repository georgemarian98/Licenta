#pragma once
#include <vector>	
#include <glm/glm.hpp>

#include "glad/glad.h"
#include "Renderer/VertexData.h"

class VertexArray{

public:
	VertexArray( ) = default;
	VertexArray(const std::vector<Vertex>& Vertices, const std::vector<uint32_t>& Indices);
	VertexArray(VertexArray&& Other);
	~VertexArray( );

	void Bind( );
	void Unbind( );

private:
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
};