#include "pch.h"
#include "VertexArray.h"

VertexArray::VertexArray(const std::vector<Vertex>& Vertices, const std::vector<GLuint>& Indices)
{
	// Create buffers/arrays
	glGenVertexArrays(1, &this->m_VAO);
	glGenBuffers(1, &this->m_VBO);
	glGenBuffers(1, &this->m_EBO);

	glBindVertexArray(this->m_VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size( ) * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size( ) * sizeof(GLuint), &Indices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

VertexArray::~VertexArray( )
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::Bind( )
{
	glBindVertexArray(m_VAO);
}

void VertexArray::Unbind( )
{
	glBindVertexArray(0);
}
