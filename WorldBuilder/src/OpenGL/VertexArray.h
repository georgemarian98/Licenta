#include "glad/glad.h"
#include <vector>	
#include <glm/glm.hpp>

struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class VertexArray{

public:
	VertexArray( ) = default;
	VertexArray(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
	~VertexArray( );

	void Bind( );
	void Unbind( );

private:
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
};