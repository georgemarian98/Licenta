#pragma once
#include <vector>	

#include "Renderer/VertexData.h"

namespace SceneEditor{
	class VertexArray{

	public:
		VertexArray( ) = default;
		VertexArray(const std::vector<Vertex>& Vertices, const std::vector<uint32_t>& Indices);
		VertexArray(VertexArray&& Other) noexcept;
		~VertexArray( );

		void Bind( );
		void Unbind( );

	private:
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
		GLuint m_EBO = 0;
	};

}